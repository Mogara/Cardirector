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
#include "croom.h"
#include "cserver.h"
#include "cserverplayer.h"
#include "ctcpsocket.h"

#include <QCoreApplication>
#include <QDateTime>

static QHash<int, CPacketRouter::Callback> interactions;
static QHash<int, CPacketRouter::Callback> callbacks;

class CServerPlayerPrivate
{
public:
    CPacketRouter *router;
    CServer *server;
    CRoom *room;

    int requestCommand;
    QVariant requestData;

    int networkDelayTestId;
    QDateTime networkDelayStartTime;
};

CServerPlayer::CServerPlayer(CTcpSocket *socket, CServer *server)
    : CAbstractPlayer(server)
{
    p_ptr = new CServerPlayerPrivate;
    p_ptr->server = server;

    p_ptr->router = new CPacketRouter(this, socket, server->packetParser());
    p_ptr->router->setInteractions(&interactions);
    p_ptr->router->setCallbacks(&callbacks);
    connect(p_ptr->router, &CPacketRouter::unknownPacket, this, &CServerPlayer::handleUnknownPacket);
    connect(p_ptr->router, &CPacketRouter::replyReady, this, &CServerPlayer::replyReady);
    connect(socket, &CTcpSocket::disconnected, this, &CServerPlayer::disconnected);

    p_ptr->room = NULL;
    p_ptr->networkDelayTestId = 0;
}

CServerPlayer::~CServerPlayer()
{
    delete p_ptr;
}

void CServerPlayer::setSocket(CTcpSocket *socket)
{
    p_ptr->router->setSocket(socket);
}

CServer *CServerPlayer::server() const
{
    return p_ptr->server;
}

CRoom *CServerPlayer::room() const
{
    return p_ptr->room;
}

void CServerPlayer::setRoom(CRoom *room)
{
    p_ptr->room = room;
}

void CServerPlayer::signup(const QString &username, const QString &password, const QString &screenName, const QString &avatar)
{
    //@to-do: check if the username is duplicated in the database.
    //@to-do: encrypt the password
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

void CServerPlayer::updateNetworkDelay()
{
    p_ptr->networkDelayTestId = qrand();
    p_ptr->networkDelayStartTime = QDateTime::currentDateTime();
    notify(S_COMMAND_NETWORK_DELAY, p_ptr->networkDelayTestId);
}

void CServerPlayer::request(int command, const QVariant &data, int timeout)
{
    p_ptr->router->request(command, data, timeout);
}

void CServerPlayer::reply(int command, const QVariant &data)
{
    p_ptr->router->reply(command, data);
}

void CServerPlayer::notify(int command, const QVariant &data)
{
    p_ptr->router->notify(command, data);
}

void CServerPlayer::prepareRequest(int command, const QVariant &data)
{
    p_ptr->requestCommand = command;
    p_ptr->requestData = data;
}

void CServerPlayer::executeRequest(int timeout)
{
    request(p_ptr->requestCommand, p_ptr->requestData, timeout);
}

void CServerPlayer::cancelRequest()
{
    p_ptr->router->cancelRequest();
}

QVariant CServerPlayer::waitForReply()
{
    return p_ptr->router->waitForReply();
}

QVariant CServerPlayer::waitForReply(int timeout)
{
    return p_ptr->router->waitForReply(timeout);
}

QVariant CServerPlayer::briefIntroduction() const
{
    QVariantList arguments;
    arguments << id();
    arguments << screenName();
    arguments << avatar();
    return arguments;
}

void CServerPlayer::AddInteraction(int command, void (*callback)(QObject *, const QVariant &))
{
    interactions.insert(command, callback);
}

void CServerPlayer::AddCallback(int command, void (*callback)(QObject *, const QVariant &))
{
    callbacks.insert(command, callback);
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

void CServerPlayer::CreateRoomCommand(QObject *receiver, const QVariant &data)
{
    CServerPlayer *player = qobject_cast<CServerPlayer *>(receiver);
    CServer *server = player->server();
    server->createRoom(player, data);
}

void CServerPlayer::EnterRoomCommand(QObject *receiver, const QVariant &data)
{
    CServerPlayer *player = qobject_cast<CServerPlayer *>(receiver);
    CServer *server = player->server();
    uint roomId = data.toUInt();
    CRoom *room = server->findRoom(roomId);
    if (room)
        room->addPlayer(player);
}

void CServerPlayer::NetworkDelayCommand(QObject *receiver, const QVariant &data)
{
    CServerPlayer *player = qobject_cast<CServerPlayer *>(receiver);
    CServerPlayerPrivate *p_ptr = player->p_ptr;
    if (p_ptr->networkDelayTestId != 0 && p_ptr->networkDelayTestId == data.toInt()) {
        player->setNetworkDelay(p_ptr->networkDelayStartTime.secsTo(QDateTime::currentDateTime()));
        p_ptr->networkDelayTestId = 0;
    }
}

void CServerPlayer::SetRoomListCommand(QObject *receiver, const QVariant &)
{
    CServerPlayer *player = qobject_cast<CServerPlayer *>(receiver);
    CServer *server = player->server();
    server->updateRoomList(player);
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

void CServerPlayer::Init()
{
    AddCallback(S_COMMAND_CHECK_VERSION, &CheckVersionCommand);
    AddCallback(S_COMMAND_SIGNUP, &SignupCommand);
    AddCallback(S_COMMAND_LOGIN, &LoginCommand);
    AddCallback(S_COMMAND_LOGOUT, &LogoutCommand);
    AddCallback(S_COMMAND_SPEAK, &SpeakCommand);
    AddCallback(S_COMMAND_CREATE_ROOM, &CreateRoomCommand);
    AddCallback(S_COMMAND_ENTER_ROOM, &EnterRoomCommand);
    AddCallback(S_COMMAND_NETWORK_DELAY, &NetworkDelayCommand);
    AddCallback(S_COMMAND_SET_ROOM_LIST, &SetRoomListCommand);
}
C_INITIALIZE_CLASS(CServerPlayer)
