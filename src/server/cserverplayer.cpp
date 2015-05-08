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
#include "cprotocol.h"
#include "cserver.h"
#include "cserverplayer.h"
#include "ctcpsocket.h"

#include <QCoreApplication>

class CServerPlayerPrivate
{
public:
    CPacketRouter *router;
    CServer *server;
    CRoom *room;
};

CServerPlayer::CServerPlayer(CTcpSocket *socket, CServer *server)
    : CAbstractPlayer(server)
{
    p_ptr = new CServerPlayerPrivate;
    p_ptr->server = server;

    p_ptr->router = new CPacketRouter(this, socket, server->packetParser());
    connect(p_ptr->router, &CPacketRouter::unknownPacket, this, &CServerPlayer::handleUnknownPacket);
    connect(socket, &CTcpSocket::disconnected, this, &CServerPlayer::disconnected);
    initCallbacks();

    p_ptr->room = NULL;
}

void CServerPlayer::initCallbacks()
{
    p_ptr->router->addCallback(S_COMMAND_CHECK_VERSION, &CheckVersionCommand);
    p_ptr->router->addCallback(S_COMMAND_SIGNUP, &SignupCommand);
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

CRoom *CServerPlayer::room() const
{
    return p_ptr->room;
}

void CServerPlayer::signup(const QString &username, const QString &password, const QString &screenName, const QString &avatar)
{
    //@to-do: check if the username is duplicated in the database.
    //@to-do: encrypt the password

    C_UNUSED(username);
    C_UNUSED(password);

    static uint playerId = 0;
    playerId++;
    setId(playerId);

    setScreenName(screenName);
    setAvatar(avatar);

    login(username, password);
}

void CServerPlayer::login(const QString &username, const QString &password)
{
    //@todo: implement this after the database is ready
    C_UNUSED(username);
    C_UNUSED(password);

    setState(Online);
    notify(S_COMMAND_LOGIN, briefIntroduction());
}

void CServerPlayer::logout()
{
    setState(Invalid);
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

QVariant CServerPlayer::briefIntroduction() const
{
    QVariantList arguments;
    arguments << id();
    arguments << screenName();
    arguments << avatar();
    return arguments;
}

/* Callbacks */

void CServerPlayer::CheckVersionCommand(QObject *receiver, const QVariant &data)
{
    C_UNUSED(receiver);
    C_UNUSED(data);
}

void CServerPlayer::SignupCommand(QObject *receiver, const QVariant &data)
{
    QVariantList arguments(data.toList());
    if (arguments.length() < 4)
        return;

    QString account = arguments.at(0).toString();
    QString password = arguments.at(1).toString();
    QString screenName = arguments.at(2).toString();
    QString avatar = arguments.at(3).toString();

    CServerPlayer *player = qobject_cast<CServerPlayer *>(receiver);
    player->signup(account, password, screenName, avatar);
}

//currently unused
void CServerPlayer::LoginCommand(QObject *receiver, const QVariant &data)
{
    C_UNUSED(receiver);
    C_UNUSED(data);

    /*QVariantList dataList(data.toList());
    if (dataList.size() >= 2) {
        QString account = dataList.at(0).toString();
        QString password = dataList.at(1).toString();

        //@to-do: implement this after database is ready
        CServerPlayer *player = qobject_cast<CServerPlayer *>(receiver);
        player->login(account, password);
    }*/
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
    if (!message.isEmpty())
        player->speak(message);
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