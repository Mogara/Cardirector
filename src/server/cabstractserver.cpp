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
#include "cabstractserver.h"
#include "cabstractserverplayer.h"
#include "ctcpserver.h"
#include "ctcpsocket.h"

class CAbstractServerPrivate
{
public:
    bool acceptMultipleClientsBehindOneIp;
    QHash<QHostAddress, CAbstractServerPlayer *> clientIp;
    CTcpServer *server;

    QList<CAbstractServerPlayer *> players;
};

CAbstractServer::CAbstractServer(QObject *parent)
    : QObject(parent)
    , p_ptr(new CAbstractServerPrivate)
{
    p_ptr->acceptMultipleClientsBehindOneIp = true;
    p_ptr->server = new CTcpServer(this);
    connect(p_ptr->server, &CTcpServer::newSocket, this, &CAbstractServer::handleNewConnection);
}

CAbstractServer::~CAbstractServer()
{
    delete p_ptr;
}

bool CAbstractServer::listen(const QHostAddress &address, ushort port)
{
    return p_ptr->server->listen(address, port);
}

void CAbstractServer::setAcceptMultipleClientsBehindOneIp(bool enabled)
{
    p_ptr->acceptMultipleClientsBehindOneIp = enabled;
}

bool CAbstractServer::acceptMultipleClientsBehindOneIp() const
{
    return p_ptr->acceptMultipleClientsBehindOneIp;
}

const QList<CAbstractServerPlayer *> &CAbstractServer::players() const
{
    return p_ptr->players;
}

CAbstractServerPlayer *CAbstractServer::createPlayer(CTcpSocket *client)
{
    return new CAbstractServerPlayer(client, this);
}

void CAbstractServer::handleNewConnection(CTcpSocket *client)
{
    CAbstractServerPlayer *player = createPlayer(client);
    if (player == NULL)
        return;

    if (!acceptMultipleClientsBehindOneIp()) {
        if (p_ptr->clientIp.contains(player->ip())) {
            CAbstractServerPlayer *prevPlayer = p_ptr->clientIp.value(player->ip());
            prevPlayer->kick();
            prevPlayer->deleteLater();
        }

        p_ptr->clientIp.insert(player->ip(), player);
    }

    //@sign-up
    p_ptr->players << player;
    emit newPlayer(player);
}
