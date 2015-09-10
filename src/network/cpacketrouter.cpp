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

#include <QSemaphore>
#include <QDateTime>
#include <QMutexLocker>

class CPacketRouterPrivate
{
public:
    const QHash<int, CPacketRouter::Callback> *interactions; //For requests
    const QHash<int, CPacketRouter::Callback> *callbacks;    //For notifications

    QObject *receiver;
    CTcpSocket *socket;
    CAbstractPacketParser *parser;

    //For the request receiver
    int requestId;
    int requestTimeout;

    //For the request sender
    QDateTime requestStartTime;
    QMutex replyMutex;
    int expectedReplyId;
    int replyTimeout;
    QVariant reply;
    QSemaphore replyReadySemaphore;
    QSemaphore *extraReplyReadySemaphore;
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
    p_ptr->expectedReplyId = -1;
    p_ptr->replyTimeout = 0;
    p_ptr->extraReplyReadySemaphore = NULL;
    setSocket(socket);
}

CPacketRouter::~CPacketRouter()
{
    abortRequest();
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
        connect(this, &CPacketRouter::messageReady, p_ptr->socket, &CTcpSocket::writePacket);
        connect(p_ptr->socket, &CTcpSocket::newPacket, this, &CPacketRouter::handlePacket);
        connect(p_ptr->socket, &CTcpSocket::disconnected, this, &CPacketRouter::abortRequest);
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

void CPacketRouter::setReplyReadySemaphore(QSemaphore *semaphore)
{
    p_ptr->extraReplyReadySemaphore = semaphore;
}

void CPacketRouter::request(int command, const QVariant &data, int timeout)
{
    //In case a request is called without a following waitForReply call
    if (p_ptr->replyReadySemaphore.available() > 0)
        p_ptr->replyReadySemaphore.acquire(p_ptr->replyReadySemaphore.available());

    static int requestId = 0;
    requestId++;

    p_ptr->replyMutex.lock();
    p_ptr->expectedReplyId = requestId;
    p_ptr->replyTimeout = timeout;
    p_ptr->requestStartTime = QDateTime::currentDateTime();
    p_ptr->replyMutex.unlock();

    QVariantList body;
    body << requestId;
    body << timeout;
    body << data;

    CPacket packet(command, CPacket::TYPE_REQUEST);
    packet.setData(body);
    emit messageReady(p_ptr->parser->parse(packet));
}

void CPacketRouter::reply(int command, const QVariant &data)
{
    QVariantList body;
    body << p_ptr->requestId;
    body << data;

    CPacket packet(command, CPacket::TYPE_REPLY);
    packet.setData(body);
    emit messageReady(p_ptr->parser->parse(packet));
}

void CPacketRouter::notify(int command, const QVariant &data)
{
    CPacket packet(command, CPacket::TYPE_NOTIFICATION);
    packet.setData(data);
    emit messageReady(p_ptr->parser->parse(packet));
}

int CPacketRouter::requestTimeout() const
{
    return p_ptr->requestTimeout;
}

//Used by a waiting object, to cancel the latest request from it
void CPacketRouter::cancelRequest()
{
    p_ptr->replyMutex.lock();
    p_ptr->expectedReplyId = -1;
    p_ptr->replyTimeout = 0;
    p_ptr->extraReplyReadySemaphore = NULL;
    p_ptr->replyMutex.unlock();

    if (p_ptr->replyReadySemaphore.available() > 0)
        p_ptr->replyReadySemaphore.acquire(p_ptr->replyReadySemaphore.available());
}

QVariant CPacketRouter::waitForReply()
{
    p_ptr->replyReadySemaphore.acquire();
    return p_ptr->reply;
}

QVariant CPacketRouter::waitForReply(int timeout)
{
    if (p_ptr->replyReadySemaphore.tryAcquire(1, timeout))
        return p_ptr->reply;
    else
        return QVariant();
}

//Force all waiting objects to wake up
void CPacketRouter::abortRequest()
{
    p_ptr->replyMutex.lock();
    if (p_ptr->expectedReplyId != -1) {
        p_ptr->replyReadySemaphore.release();
        if (p_ptr->extraReplyReadySemaphore)
            p_ptr->extraReplyReadySemaphore->release();
        p_ptr->expectedReplyId = -1;
        p_ptr->extraReplyReadySemaphore = NULL;
    }
    p_ptr->replyMutex.unlock();
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
        if (dataList.size() != 3)
            return;

        p_ptr->requestId = dataList.at(0).toInt();
        p_ptr->requestTimeout = dataList.at(1).toInt();

        (*func)(p_ptr->receiver, dataList.at(2));

    } else if (packet.type() == CPacket::TYPE_REPLY) {
        QVariantList dataList(packet.data().toList());

        QMutexLocker locker(&p_ptr->replyMutex);
        if (dataList.size() != 2 || p_ptr->expectedReplyId != dataList.at(0).toInt())
            return;

        p_ptr->expectedReplyId = -1;

        if (0 <= p_ptr->replyTimeout && p_ptr->replyTimeout < p_ptr->requestStartTime.secsTo(QDateTime::currentDateTime()))
            return;

        p_ptr->reply = dataList.at(1);
        if (p_ptr->callbacks) {
            Callback func = p_ptr->callbacks->value(packet.command());
            if (func != NULL)
                (*func)(p_ptr->receiver, p_ptr->reply);
        }
        p_ptr->replyReadySemaphore.release();
        if (p_ptr->extraReplyReadySemaphore) {
            p_ptr->extraReplyReadySemaphore->release();
            p_ptr->extraReplyReadySemaphore = NULL;
        }
        locker.unlock();
        emit replyReady();
    }
}
