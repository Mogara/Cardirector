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

#include "cpch.h"
#include "cclient.h"
#include "cpacketrouter.h"
#include "ctcpsocket.h"
#include "cjsonpacketparser.h"
#include "cprotocol.h"
#include "cclientplayer.h"

class CClientPrivate
{
public:
    CPacketRouter *router;
    QMap<uint, CClientPlayer *> players;
    CClientPlayer *self;
};

CClient::CClient(QObject *parent)
    : QObject(parent)
    , p_ptr(new CClientPrivate)
{
    initCallbacks();
    p_ptr->self = NULL;
}

CClient::~CClient()
{
    delete p_ptr;
}

void CClient::initCallbacks()
{
    CTcpSocket *socket = new CTcpSocket;
    p_ptr->router = new CPacketRouter(this, socket, new CJsonPacketParser);
    connect(socket, &CTcpSocket::connected, this, &CClient::connected);

    addCallback(S_COMMAND_SPEAK, &SpeakCommand);
    addCallback(S_COMMAND_SET_PLAYER_LIST, &SetPlayerListCommand);
    addCallback(S_COMMAND_ADD_PLAYER, &AddPlayerCommand);
    addCallback(S_COMMAND_REMOVE_PLAYER, &RemovePlayerCommand);
    addCallback(S_COMMAND_LOGIN, &LoginCommand);
}

void CClient::addCallback(int command, void (*callback)(QObject *, const QVariant &))
{
    p_ptr->router->addCallback(command, callback);
}

void CClient::connectToHost(const QHostAddress &server, ushort port)
{
    CTcpSocket *socket = p_ptr->router->socket();
    socket->connectToHost(server, port);
}

void CClient::signup(const QString &username, const QString &password, const QString &screenName, const QString &avatar)
{
    QVariantList arguments;
    arguments << username << password << screenName << avatar;
    notifyServer(S_COMMAND_SIGNUP, arguments);
}

void CClient::login(const QString &username, const QString &password)
{
    QVariantList arguments;
    arguments << username << password;
    notifyServer(S_COMMAND_LOGIN, arguments);
}

void CClient::requestServer(int command, const QVariant &data)
{
    p_ptr->router->request(command, data);
}

void CClient::replyToServer(int command, const QVariant &data)
{
    p_ptr->router->reply(command, data);
}

void CClient::notifyServer(int command, const QVariant &data)
{
    p_ptr->router->notify(command, data);
}

const CClientPlayer *CClient::findPlayer(uint id) const
{
    return p_ptr->players.value(id);
}

QList<const CClientPlayer *> CClient::players() const
{
    QList<const CClientPlayer *> playerList;
    foreach (const CClientPlayer *player, p_ptr->players)
        playerList << player;
    return playerList;
}

CClientPlayer *CClient::findPlayer(uint id)
{
    return p_ptr->players.value(id);
}

CClientPlayer *CClient::self() const
{
    return p_ptr->self;
}

CClientPlayer *CClient::addPlayer(const QVariant &data)
{
    QVariantList arguments = data.toList();
    if (arguments.length() < 3)
        return NULL;

    uint playerId = arguments.at(0).toUInt();
    if (playerId > 0) {
        CClientPlayer *player = new CClientPlayer(playerId, this);
        player->setScreenName(arguments.at(1).toString());
        player->setAvatar(arguments.at(2).toString());

        p_ptr->players.insert(playerId, player);
        return player;
    }

    return NULL;
}

/* Callbacks */

void CClient::SetPlayerListCommand(QObject *receiver, const QVariant &data)
{
    QVariantList playerList(data.toList());
    CClient *client = qobject_cast<CClient *>(receiver);

    if (!client->p_ptr->players.isEmpty()) {
        foreach (CClientPlayer *player, client->p_ptr->players)
            player->deleteLater();
        client->p_ptr->players.clear();
    }

    foreach (const QVariant &player, playerList)
        client->addPlayer(player);
}

void CClient::AddPlayerCommand(QObject *receiver, const QVariant &data)
{
    CClient *client = qobject_cast<CClient *>(receiver);
    CClientPlayer *player = client->addPlayer(data);
    emit client->playerAdded(player);
}

void CClient::RemovePlayerCommand(QObject *receiver, const QVariant &data)
{
    CClient *client = qobject_cast<CClient *>(receiver);
    uint playerId = data.toUInt();
    CClientPlayer *player = client->findPlayer(playerId);
    if (player != NULL) {
        emit client->playerRemoved(player);
        client->p_ptr->players.remove(playerId);
        player->deleteLater();
    }
}

void CClient::LoginCommand(QObject *receiver, const QVariant &data)
{
    CClient *client = qobject_cast<CClient *>(receiver);
    client->p_ptr->self = client->addPlayer(data);
    emit client->loggedIn();
}

void CClient::SpeakCommand(QObject *receiver, const QVariant &data)
{
    QVariantList arguments = data.toList();
    if (arguments.size() < 2)
        return;

    CClient *client = qobject_cast<CClient *>(receiver);
    CClientPlayer *player = client->findPlayer(arguments.at(0).toUInt());
    if (player != NULL) {
        QString message = arguments.at(1).toString();
        player->speak(message);
    }
}
