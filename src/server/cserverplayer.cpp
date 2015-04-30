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

#include "cserverplayer.h"
#include "ctcpsocket.h"
#include "cjsonpacketparser.h"
#include "cprotocol.h"
#include "cpacketrouter.h"

class CServerPlayerPrivate
{
public:
    CPacketRouter *router;
};

CServerPlayer::CServerPlayer(QObject *parent)
    : CAbstractPlayer(parent)
{
    init();
}

CServerPlayer::CServerPlayer(CTcpSocket *socket, QObject *parent)
    : CAbstractPlayer(parent)
{
    init();
    p_ptr->router->setSocket(socket);
}

void CServerPlayer::init()
{
    p_ptr = new CServerPlayerPrivate;
    p_ptr->router = new CPacketRouter(this, new CTcpSocket, new CJsonPacketParser);
    initCallbacks();
}

void CServerPlayer::initCallbacks()
{
    p_ptr->router->addCallback(S_COMMAND_CHECK_VERSION, &CheckVersionCommand);
    p_ptr->router->addCallback(S_COMMAND_LOGIN, &LoginCommand);
    p_ptr->router->addCallback(S_COMMAND_LOGOUT, &LogoutCommand);
    p_ptr->router->addCallback(S_COMMAND_SPEAK, &SpeakCommand);
}

CServerPlayer::~CServerPlayer()
{
    delete p_ptr;
}

void CServerPlayer::setSocket(CTcpSocket *socket)
{
    p_ptr->router->setSocket(socket);
}

void CServerPlayer::kick()
{
    //@to-do: send a warning
    p_ptr->router->socket()->disconnectFromHost();
}

QHostAddress CServerPlayer::ip() const
{
    return p_ptr->router->socket()->peerAddress();
}

void CServerPlayer::request(int command, const QVariant &data)
{
    p_ptr->router->request(command, data);
}

void CServerPlayer::reply(int command, const QVariant &data)
{
    p_ptr->router->reply(command, data);
}

void CServerPlayer::notify(int command, const QVariant &data)
{
    p_ptr->router->notify(command, data);
}

QVariant CServerPlayer::waitForReply()
{
    return p_ptr->router->waitForReply();
}

/* Callbacks */

void CServerPlayer::CheckVersionCommand(QObject *player, const QVariant &data)
{
    C_UNUSED(player);
    C_UNUSED(data);
}

void CServerPlayer::LoginCommand(QObject *player, const QVariant &data)
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

void CServerPlayer::LogoutCommand(QObject *player, const QVariant &)
{
    C_UNUSED(player);
    //@to-do: handle logout command, without which the disconnection is unexpected
}

void CServerPlayer::SpeakCommand(QObject *receiver, const QVariant &data)
{
    CServerPlayer *player = qobject_cast<CServerPlayer *>(receiver);
    QString message = data.toString();
    if (!message.isEmpty()) {
        //@to-do: broadcast the message to all the clients
        qDebug() << player->screenName() << message;
    }
}
