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
#include "network/ctcpserver.h"
#include "network/ctcpsocket.h"

CAbstractServer::CAbstractServer(QObject *parent)
    : QObject(parent)
    , m_acceptMultipleClientsBehindOneIp(true)
    , m_server(new CTcpServer(this))
{
    connect(m_server, &CTcpServer::newSocket, this, &CAbstractServer::handleNewConnection);
}

void CAbstractServer::handleNewConnection(CTcpSocket *client)
{
    if (!acceptMultipleClientsBehindOneIp()) {
        if (m_clientIp.contains(client->peerAddress())) {
            //@todo: send a warning
            client->disconnectFromHost();
            client->deleteLater();
            return;
        } else {
            m_clientIp.insert(client->peerAddress());
        }
    }

    //@sign-up
}
