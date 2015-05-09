/********************************************************************
    Copyright (c) 2013-2015 - Mogara

    This file is part of Cardirector.

    This game engine is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 3.0
    of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    See the LICENSE file for more details.

    Mogara
    *********************************************************************/

#include "cpacketrouter.h"
#include "ctcpsocket.h"

#include <QMutex>

class CPacketRouterPrivate
{
public:
    const QHash<int, CPacketRouter::Callback> *interactions; //For requests
    const QHash<int, CPacketRouter::Callback> *callbacks;    //For notifications

    QObject *receiver;
    CTcpSocket *socket;
    CAbstractPacketParser *parser;
    int currentRequestId;
    int nextReplyId;
    QVariant reply;
    QMutex replyMutex;
};

CPacketRouter::CPacketRouter(QObject *receiver, CTcpSocket *socket, CAbstractPacketParser *parser)
    : QObject(receiver)
    , p_ptr(new CPacketRouterPrivate)
{
    p_ptr->interactions = NULL;
    p_ptr->callbacks = NULL;
    p_ptr->receiver = receiver;
    p_ptr->parser = parser;
    p_ptr->socket = NULL;
    setSocket(socket);
    p_ptr->replyMutex.lock();
}

CPacketRouter::~CPacketRouter()
{
    p_ptr->replyMutex.unlock();
    delete p_ptr;
}

void CPacketRouter::setSocket(CTcpSocket *socket)
{
    if (p_ptr->socket != NULL) {
        p_ptr->socket->disconnect(this);
        disconnect(p_ptr->socket);
        p_ptr->socket->deleteLater();
    }

    p_ptr->socket = socket;
    if (p_ptr->socket != NULL) {
        connect(p_ptr->socket, &CTcpSocket::newPacket, this, &CPacketRouter::handlePacket);
        p_ptr->socket->setParent(this);
    }
}

CTcpSocket *CPacketRouter::socket()
{
    return p_ptr->socket;
}

CAbstractPacketParser *CPacketRouter::parser()
{
    return p_ptr->parser;
}

void CPacketRouter::setInteractions(const QHash<int, Callback> *interactions)
{
    p_ptr->interactions = interactions;
}

void CPacketRouter::setCallbacks(const QHash<int, Callback> *callbacks)
{
    p_ptr->callbacks = callbacks;
}

void CPacketRouter::request(int command, const QVariant &data)
{
    static int requestId = 0;
    requestId++;
    p_ptr->nextReplyId = requestId;

    QVariantList body;
    body << requestId;
    body << data;

    CPacket packet(command, CPacket::TYPE_REQUEST);
    packet.setData(body);
    p_ptr->socket->writePacket(p_ptr->parser->parse(packet));
}

void CPacketRouter::reply(int command, const QVariant &data)
{
    QVariantList body;
    body << p_ptr->currentRequestId;
    body << data;

    CPacket packet(command, CPacket::TYPE_REPLY);
    packet.setData(body);
    p_ptr->socket->writePacket(p_ptr->parser->parse(packet));
}

void CPacketRouter::notify(int command, const QVariant &data)
{
    CPacket packet(command, CPacket::TYPE_NOTIFICATION);
    packet.setData(data);
    p_ptr->socket->writePacket(p_ptr->parser->parse(packet));
}

QVariant CPacketRouter::waitForReply()
{
    p_ptr->replyMutex.lock();
    return p_ptr->reply;
}

void CPacketRouter::handlePacket(const QByteArray &rawPacket)
{
    if (p_ptr->receiver == NULL)
        return;

    CPacket packet = p_ptr->parser->parse(rawPacket);
    if (!packet.isValid()) {
        emit unknownPacket(rawPacket);
        return;
    }

    if (packet.type() == CPacket::TYPE_NOTIFICATION) {
        if (p_ptr->callbacks == NULL)
            return;
        Callback func = p_ptr->callbacks->value(packet.command());
        if (func)
            (*func)(p_ptr->receiver, packet.data());

    } else if (packet.type() == CPacket::TYPE_REQUEST) {
        if (p_ptr->interactions == NULL)
            return;
        Callback func = p_ptr->interactions->value(packet.command());
        if (func == NULL)
            return;

        QVariantList dataList(packet.data().toList());
        if (dataList.size() != 2)
            return;

        p_ptr->currentRequestId = dataList.at(0).toInt();

        (*func)(p_ptr->receiver, dataList.at(1));

    } else if (packet.type() == CPacket::TYPE_REPLY) {
        QVariantList dataList(packet.data().toList());
        if (dataList.size() != 2)
            return;

        if (p_ptr->nextReplyId != dataList.at(0).toInt())
            return;

        p_ptr->reply = dataList.at(1);
        if (p_ptr->callbacks) {
            Callback func = p_ptr->callbacks->value(packet.command());
            if (func != NULL)
                (*func)(p_ptr->receiver, p_ptr->reply);
        }
        p_ptr->replyMutex.unlock();
    }
}
