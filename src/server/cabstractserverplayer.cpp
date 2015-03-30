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

#include "cabstractserverplayer.h"
#include "ctcpsocket.h"

class CAbstractServerPlayerPrivate
{
public:
    CTcpSocket *socket;
};

CAbstractServerPlayer::CAbstractServerPlayer(QObject *parent)
    : CAbstractPlayer(parent)
    , p_ptr(new CAbstractServerPlayerPrivate)
{
    p_ptr->socket = NULL;
}

CAbstractServerPlayer::CAbstractServerPlayer(CTcpSocket *socket, QObject *parent)
    : CAbstractPlayer(parent)
    , p_ptr(new CAbstractServerPlayerPrivate)
{
    p_ptr->socket = socket;
}

CAbstractServerPlayer::~CAbstractServerPlayer()
{
    delete p_ptr;
}

void CAbstractServerPlayer::setSocket(CTcpSocket *socket)
{
    if (p_ptr->socket != NULL) {
        p_ptr->socket->disconnect(this);
        this->disconnect(p_ptr->socket);
        p_ptr->socket->deleteLater();
    }

    p_ptr->socket = socket;
    if (socket != NULL) {
        connect(socket, &CTcpSocket::disconnected, this, &CAbstractServerPlayer::disconnected);
        connect(socket, &CTcpSocket::newPacket, this, &CAbstractServerPlayer::handleNewPacket);
    }
}

void CAbstractServerPlayer::handleNewPacket(const QByteArray &rawPacket)
{
    //@to-do: handle new packet
}
