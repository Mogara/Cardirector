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

#include "cabstractserverplayer.h"
#include "ctcpsocket.h"
#include "cjsonpacketparser.h"
#include "cprotocol.h"

class CAbstractServerPlayerPrivate
{
public:
    CTcpSocket *socket;
    CAbstractPacketParser *packetParser;

    static QMap<int, CAbstractServerPlayer::Callback> interactions; //For requests
    static QMap<int, CAbstractServerPlayer::Callback> callbacks;    //For notifications
};

QMap<int, CAbstractServerPlayer::Callback> CAbstractServerPlayerPrivate::interactions;
QMap<int, CAbstractServerPlayer::Callback> CAbstractServerPlayerPrivate::callbacks;

CAbstractServerPlayer::CAbstractServerPlayer(QObject *parent)
    : CAbstractPlayer(parent)
{
    init();
    p_ptr->socket = NULL;
}

CAbstractServerPlayer::CAbstractServerPlayer(CTcpSocket *socket, QObject *parent)
    : CAbstractPlayer(parent)
{
    init();
    p_ptr->socket = socket;
}

void CAbstractServerPlayer::init()
{
    p_ptr = new CAbstractServerPlayerPrivate;
    p_ptr->packetParser = new CJsonPacketParser;

    if (p_ptr->interactions.isEmpty() && p_ptr->callbacks.isEmpty()) {
        initCallbacks();
    }
}

void CAbstractServerPlayer::initCallbacks()
{
    p_ptr->callbacks[S_COMMAND_CHECK_VERSION] = &CAbstractServerPlayer::checkVersionCommand;
    p_ptr->callbacks[S_COMMAND_LOGIN] = &CAbstractServerPlayer::loginCommand;
    p_ptr->callbacks[S_COMMAND_LOGOUT] = &CAbstractServerPlayer::logoutCommand;
}

CAbstractServerPlayer::~CAbstractServerPlayer()
{
    delete p_ptr->packetParser;
    delete p_ptr;
}

void CAbstractServerPlayer::setSocket(CTcpSocket *socket)
{
    if (p_ptr->socket != NULL) {
        p_ptr->socket->disconnect(this);
        this->disconnect(p_ptr->socket);
        p_ptr->socket->deleteLater();
    }

    p_ptr->socket = socket;
    if (socket != NULL) {
        connect(socket, &CTcpSocket::disconnected, this, &CAbstractServerPlayer::disconnected);
        connect(socket, &CTcpSocket::newPacket, this, &CAbstractServerPlayer::handleNewPacket);
    }
}

void CAbstractServerPlayer::setPacketParser(CAbstractPacketParser *parser)
{
    if (p_ptr->packetParser)
        delete p_ptr->packetParser;
    p_ptr->packetParser = parser;
}

void CAbstractServerPlayer::kick()
{
    //@to-do: send a warning
    p_ptr->socket->disconnectFromHost();
}

QHostAddress CAbstractServerPlayer::ip() const
{
    return p_ptr->socket->peerAddress();
}

void CAbstractServerPlayer::handleNewPacket(const QByteArray &rawPacket)
{
    CPacket packet = p_ptr->packetParser->parse(rawPacket);
    if (packet.isValid())
        return;

    if (packet.type() == CPacket::TYPE_NOTIFICATION) {
        Callback func = p_ptr->callbacks.value(packet.type());
        if (func)
            (this->*func)(packet.data());
    } else if (packet.type() == CPacket::TYPE_REQUEST) {
        Callback func = p_ptr->interactions.value(packet.type());
        if (func)
            (this->*func)(packet.data());
    }
}

void CAbstractServerPlayer::checkVersionCommand(const QVariant &data)
{
    C_UNUSED(data);
}

void CAbstractServerPlayer::loginCommand(const QVariant &data)
{
    QVariantList dataList(data.toList());
    if (dataList.size() >= 2) {
        QString account = dataList.at(0).toString();
        QString password = dataList.at(1).toString();

        C_UNUSED(account);
        C_UNUSED(password);

        //@to-do: implement this after database is ready
    }
}

void CAbstractServerPlayer::logoutCommand(const QVariant &)
{
    //@to-do: handle logout command, without which the disconnection is unexpected
}

void CAbstractServerPlayer::speakCommand(const QVariant &data)
{
    QString message = data.toString();
    if (!message.isEmpty()) {
        //@to-do: broadcast the message to all the clients
        qDebug() << screenName() << message;
    }
}
