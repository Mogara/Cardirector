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

#include <QCoreApplication>

class CServerPlayerPrivate
{
public:
    CPacketRouter *router;
};

CServerPlayer::CServerPlayer(CTcpSocket *socket, QObject *parent)
    : CAbstractPlayer(parent)
{
    p_ptr = new CServerPlayerPrivate;

    p_ptr->router = new CPacketRouter(this, socket, new CJsonPacketParser);
    connect(p_ptr->router, &CPacketRouter::unknownPacket, this, &CServerPlayer::handleUnknownPacket);
    connect(socket, &CTcpSocket::disconnected, this, &CServerPlayer::disconnected);
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

void CServerPlayer::logout()
{
    setState(LoggedOut);
    p_ptr->router->socket()->disconnectFromHost();
}

void CServerPlayer::kick()
{
    //@to-do: send a warning
    logout();
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

void CServerPlayer::LoginCommand(QObject *receiver, const QVariant &data)
{
    QVariantList dataList(data.toList());
    if (dataList.size() >= 2) {
        QString account = dataList.at(0).toString();
        QString password = dataList.at(1).toString();

        C_UNUSED(account);
        C_UNUSED(password);

        //@to-do: implement this after database is ready
        CServerPlayer *player = qobject_cast<CServerPlayer *>(receiver);
        player->setState(Online);
    }
}

void CServerPlayer::LogoutCommand(QObject *receiver, const QVariant &)
{
    CServerPlayer *player = qobject_cast<CServerPlayer *>(receiver);
    player->logout();
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

void CServerPlayer::handleUnknownPacket(const QByteArray &packet)
{
    //Handle requests from a browser
    if (packet.startsWith("GET") || packet.startsWith("POST")) {
        CTcpSocket *socket = p_ptr->router->socket();

        //Read the whole HTTP request
        while (socket->canReadLine()) {
            QByteArray line = socket->readLine();
            if (line.isEmpty() || line.at(0) == '\n' || line.at(0) == '\r')
                break;
        }

        QString scheme = qApp->applicationName();
        scheme.remove(QRegExp("[^A-Za-z]"));
        QString location = QString("Location: %1://%2:%3/\r\n");
        location = location.arg(scheme).arg(socket->localAddress().toString()).arg(socket->localPort());

        socket->write("HTTP/1.1 302 Moved Temporarily\r\n");
        socket->write("Server: Cardirector\r\n");
        socket->write(location.toLatin1());
        socket->write("Connection: close\r\n");
        socket->write("\r\n");

        socket->disconnectFromHost();
    }
}
