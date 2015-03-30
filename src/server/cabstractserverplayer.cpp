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
#include "cpacketrouter.h"

class CAbstractServerPlayerPrivate
{
public:
    CTcpSocket *socket;
    CAbstractPacketParser *packetParser;
    CPacketRouter<CAbstractServerPlayer> *router;
};

CAbstractServerPlayer::CAbstractServerPlayer(QObject *parent)
    : CAbstractPlayer(parent)
{
    init();
}

CAbstractServerPlayer::CAbstractServerPlayer(CTcpSocket *socket, QObject *parent)
    : CAbstractPlayer(parent)
{
    init();
    setSocket(socket);
}

void CAbstractServerPlayer::init()
{
    p_ptr = new CAbstractServerPlayerPrivate;
    p_ptr->socket = NULL;
    p_ptr->packetParser = new CJsonPacketParser;
    initCallbacks();
}

void CAbstractServerPlayer::initCallbacks()
{
    p_ptr->router = new CPacketRouter<CAbstractServerPlayer>(this, p_ptr->packetParser);
    p_ptr->router->addCallback(S_COMMAND_CHECK_VERSION, &CheckVersionCommand);
    p_ptr->router->addCallback(S_COMMAND_LOGIN, &LoginCommand);
    p_ptr->router->addCallback(S_COMMAND_LOGOUT, &LogoutCommand);
    p_ptr->router->addCallback(S_COMMAND_SPEAK, &SpeakCommand);
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
        connect(socket, &CTcpSocket::newPacket, p_ptr->router, &CPacketRouter<CAbstractServerPlayer>::handlePacket);
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

void CAbstractServerPlayer::notify(int command, const QVariant &data)
{
    CPacket packet(command, CPacket::TYPE_NOTIFICATION);
    packet.setData(data);
    p_ptr->socket->writePacket(p_ptr->packetParser->parse(packet));
}

/* Callbacks */

void CAbstractServerPlayer::CheckVersionCommand(CAbstractServerPlayer *player, const QVariant &data)
{
    C_UNUSED(player);
    C_UNUSED(data);
}

void CAbstractServerPlayer::LoginCommand(CAbstractServerPlayer *player, const QVariant &data)
{
    C_UNUSED(player);
    QVariantList dataList(data.toList());
    if (dataList.size() >= 2) {
        QString account = dataList.at(0).toString();
        QString password = dataList.at(1).toString();

        C_UNUSED(account);
        C_UNUSED(password);

        //@to-do: implement this after database is ready
    }
}

void CAbstractServerPlayer::LogoutCommand(CAbstractServerPlayer *player, const QVariant &)
{
    C_UNUSED(player);
    //@to-do: handle logout command, without which the disconnection is unexpected
}

void CAbstractServerPlayer::SpeakCommand(CAbstractServerPlayer *player, const QVariant &data)
{
    QString message = data.toString();
    if (!message.isEmpty()) {
        //@to-do: broadcast the message to all the clients
        qDebug() << player->screenName() << message;
    }
}
