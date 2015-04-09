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
#include "cabstractclient.h"
#include "cnetwork.h"

class CAbstractClientPrivate
{
public:
    CPacketRouter *router;
};

CAbstractClient::CAbstractClient(QObject *parent)
    : QObject(parent)
    , p_ptr(new CAbstractClientPrivate)
{
    initCallbacks();
}

CAbstractClient::~CAbstractClient()
{
    delete p_ptr;
}

void CAbstractClient::initCallbacks()
{
    CTcpSocket *socket = new CTcpSocket;
    p_ptr->router = new CPacketRouter(this, socket, new CJsonPacketParser);
    connect(socket, &CTcpSocket::connected, this, &CAbstractClient::connected);

    p_ptr->router->addCallback(S_COMMAND_SPEAK, &SpeakCommand);
}

void CAbstractClient::connectToHost(const QHostAddress &server, ushort port)
{
    CTcpSocket *socket = p_ptr->router->socket();
    socket->connectToHost(server, port);
}

void CAbstractClient::requestServer(int command, const QVariant &data)
{
    p_ptr->router->request(command, data);
}

void CAbstractClient::replyToServer(int command, const QVariant &data)
{
    p_ptr->router->reply(command, data);
}

void CAbstractClient::notifyServer(int command, const QVariant &data)
{
    p_ptr->router->notify(command, data);
}

/* Callbacks */

void CAbstractClient::SpeakCommand(QObject *client, const QVariant &data)
{
    qDebug() << client << data;
}
