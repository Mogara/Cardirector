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
#include "cserver.h"
#include "cserverplayer.h"
#include "ctcpserver.h"
#include "ctcpsocket.h"

class CServerPrivate
{
public:
    bool acceptMultipleClientsBehindOneIp;
    QHash<QHostAddress, CServerPlayer *> clientIp;
    CTcpServer *server;

    QList<CServerPlayer *> players;
};

CServer::CServer(QObject *parent)
    : QObject(parent)
    , p_ptr(new CServerPrivate)
{
    p_ptr->acceptMultipleClientsBehindOneIp = true;
    p_ptr->server = new CTcpServer(this);
    connect(p_ptr->server, &CTcpServer::newSocket, this, &CServer::handleNewConnection);
}

CServer::~CServer()
{
    delete p_ptr;
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

const QList<CServerPlayer *> &CServer::players() const
{
    return p_ptr->players;
}

void CServer::handleNewConnection(CTcpSocket *client)
{
    CServerPlayer *player = new CServerPlayer(client, this);
    if (player == NULL)
        return;

    if (!acceptMultipleClientsBehindOneIp()) {
        if (p_ptr->clientIp.contains(player->ip())) {
            CServerPlayer *prevPlayer = p_ptr->clientIp.value(player->ip());
            prevPlayer->kick();
            prevPlayer->deleteLater();
        }

        p_ptr->clientIp.insert(player->ip(), player);
    }

    //@sign-up
    p_ptr->players << player;
    emit newPlayer(player);
}
