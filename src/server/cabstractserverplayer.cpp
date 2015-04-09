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
    CPacketRouter *router;
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
    p_ptr->router->setSocket(socket);
}

void CAbstractServerPlayer::init()
{
    p_ptr = new CAbstractServerPlayerPrivate;
    p_ptr->router = new CPacketRouter(this, new CTcpSocket, new CJsonPacketParser);
    initCallbacks();
}

void CAbstractServerPlayer::initCallbacks()
{
    p_ptr->router->addCallback(S_COMMAND_CHECK_VERSION, &CheckVersionCommand);
    p_ptr->router->addCallback(S_COMMAND_LOGIN, &LoginCommand);
    p_ptr->router->addCallback(S_COMMAND_LOGOUT, &LogoutCommand);
    p_ptr->router->addCallback(S_COMMAND_SPEAK, &SpeakCommand);
}

CAbstractServerPlayer::~CAbstractServerPlayer()
{
    delete p_ptr;
}

void CAbstractServerPlayer::setSocket(CTcpSocket *socket)
{
    p_ptr->router->setSocket(socket);
}

void CAbstractServerPlayer::kick()
{
    //@to-do: send a warning
    p_ptr->router->socket()->disconnectFromHost();
}

QHostAddress CAbstractServerPlayer::ip() const
{
    return p_ptr->router->socket()->peerAddress();
}

void CAbstractServerPlayer::request(int command, const QVariant &data)
{
    p_ptr->router->request(command, data);
}

void CAbstractServerPlayer::reply(int command, const QVariant &data)
{
    p_ptr->router->reply(command, data);
}

void CAbstractServerPlayer::notify(int command, const QVariant &data)
{
    p_ptr->router->notify(command, data);
}

QVariant CAbstractServerPlayer::waitForReply()
{
    return p_ptr->router->waitForReply();
}

/* Callbacks */

void CAbstractServerPlayer::CheckVersionCommand(QObject *player, const QVariant &data)
{
    C_UNUSED(player);
    C_UNUSED(data);
}

void CAbstractServerPlayer::LoginCommand(QObject *player, const QVariant &data)
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

void CAbstractServerPlayer::LogoutCommand(QObject *player, const QVariant &)
{
    C_UNUSED(player);
    //@to-do: handle logout command, without which the disconnection is unexpected
}

void CAbstractServerPlayer::SpeakCommand(QObject *receiver, const QVariant &data)
{
    CAbstractServerPlayer *player = qobject_cast<CAbstractServerPlayer *>(receiver);
    QString message = data.toString();
    if (!message.isEmpty()) {
        //@to-do: broadcast the message to all the clients
        qDebug() << player->screenName() << message;
    }
}
