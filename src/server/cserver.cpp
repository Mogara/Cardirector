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
#include "croom.h"
#include "cserver.h"
#include "cserverplayer.h"
#include "ctcpserver.h"
#include "ctcpsocket.h"
#include "cprotocol.h"
#include "cjsonpacketparser.h"

class CServerPrivate
{
public:
    bool acceptMultipleClientsBehindOneIp;
    QHash<QHostAddress, CServerPlayer *> clientIp;
    CTcpServer *server;
    CAbstractPacketParser *parser;

    QHash<uint, CServerPlayer *> players;
    CRoom *lobby;
    QHash<uint, CRoom *> rooms;
};

CServer::CServer(QObject *parent)
    : QObject(parent)
    , p_ptr(new CServerPrivate)
{
    p_ptr->parser = new CJsonPacketParser;
    p_ptr->acceptMultipleClientsBehindOneIp = true;
    p_ptr->lobby = new CRoom(this);
    connect(p_ptr->lobby, &CRoom::playerAdded, this, &CServer::updateRoomList);
    p_ptr->server = new CTcpServer(this);
    connect(p_ptr->server, &CTcpServer::newSocket, this, &CServer::handleNewConnection);
}

CServer::~CServer()
{
    delete p_ptr->parser;
    delete p_ptr;
}

void CServer::setPacketParser(CAbstractPacketParser *parser)
{
    if (p_ptr->players.isEmpty()) {
        delete p_ptr->parser;
        p_ptr->parser = parser;
    } else {
        qWarning("Packet parser can't be switched after the server starts.");
    }
}

CAbstractPacketParser *CServer::packetParser() const
{
    return p_ptr->parser;
}

bool CServer::listen(const QHostAddress &address, ushort port)
{
    return p_ptr->server->listen(address, port);
}

void CServer::setAcceptMultipleClientsBehindOneIp(bool enabled)
{
    p_ptr->acceptMultipleClientsBehindOneIp = enabled;
}

bool CServer::acceptMultipleClientsBehindOneIp() const
{
    return p_ptr->acceptMultipleClientsBehindOneIp;
}

CServerPlayer *CServer::findPlayer(uint id)
{
    return p_ptr->players.value(id);
}

QHash<uint, CServerPlayer *> CServer::players() const
{
    return p_ptr->players;
}

void CServer::createRoom(CServerPlayer *owner, const QVariant &config)
{
    C_UNUSED(config);
    CRoom *room = new CRoom(this);
    connect(room, &CRoom::abandoned, this, &CServer::onRoomAbandoned);
    room->addPlayer(owner);
    room->setOwner(owner);
    p_ptr->rooms.insert(room->id(), room);
    emit roomCreated(room);
}

CRoom *CServer::findRoom(uint id) const
{
    return p_ptr->rooms.value(id);
}

QHash<uint, CRoom *> CServer::rooms() const
{
    return p_ptr->rooms;
}

CRoom *CServer::lobby() const
{
    return p_ptr->lobby;
}

void CServer::updateRoomList(CServerPlayer *player)
{
    QVariantList roomList;
    foreach (CRoom *room, p_ptr->rooms)
        roomList << room->config();
    player->notify(S_COMMAND_SET_ROOM_LIST, roomList);
}

void CServer::broadcastNotification(int command, const QVariant &data, CServerPlayer *except)
{
    foreach (CServerPlayer *player, p_ptr->players) {
        if (player != except)
            player->notify(command, data);
    }
}

void CServer::handleNewConnection(CTcpSocket *client)
{
    CServerPlayer *player = new CServerPlayer(client, this);

    if (!acceptMultipleClientsBehindOneIp()) {
        if (p_ptr->clientIp.contains(player->ip())) {
            CServerPlayer *prevPlayer = p_ptr->clientIp.value(player->ip());
            prevPlayer->kick();
            prevPlayer->deleteLater();
        }

        p_ptr->clientIp.insert(player->ip(), player);
    }

    connect(player, &CServerPlayer::disconnected, this, &CServer::onPlayerDisconnected);
    connect(player, &CServerPlayer::stateChanged, this, &CServer::onPlayerStateChanged);
}

void CServer::onRoomAbandoned()
{
    CRoom *room = qobject_cast<CRoom *>(sender());
    p_ptr->rooms.remove(room->id());
}

void CServer::onPlayerDisconnected()
{
    CServerPlayer *player = qobject_cast<CServerPlayer *>(sender());
    //@todo: check the state and enable reconnection
    //if (player && player->state() == CServerPlayer::LoggedOut)
    p_ptr->players.remove(player->id());
    player->deleteLater();
}

void CServer::onPlayerStateChanged()
{
    CServerPlayer *player = qobject_cast<CServerPlayer *>(sender());
    if (player->state() == CServerPlayer::Online) {
        if (!p_ptr->players.contains(player->id())) {
            p_ptr->players.insert(player->id(), player);
            p_ptr->lobby->addPlayer(player);
            emit playerAdded(player);
        }
    }
}
