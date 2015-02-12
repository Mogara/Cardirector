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

#include "ctcpsocket.h"

#include <QTcpSocket>
#include <QTimer>

class CInlineTextSocketPrivate
{
public:
    QTimer *keepAliveTimer;
    QTimer *checkAliveTimer;
};

CTcpSocket::CTcpSocket(QObject *parent)
    : QTcpSocket(parent)
    , p_ptr(new CInlineTextSocketPrivate)
{
    connect(this, &QTcpSocket::readyRead, this, &CTcpSocket::readPacket);

    p_ptr->keepAliveTimer = new QTimer(this);
    p_ptr->keepAliveTimer->setInterval(30000);
    connect(p_ptr->keepAliveTimer, &QTimer::timeout, this, &CTcpSocket::keepAlive);
    p_ptr->keepAliveTimer->start();

    p_ptr->checkAliveTimer = new QTimer(this);
    p_ptr->checkAliveTimer->setInterval(60000);
    connect(p_ptr->checkAliveTimer, &QTimer::timeout, this, &CTcpSocket::disconnected);
    connect(p_ptr->checkAliveTimer, &QTimer::timeout, this, &QTcpSocket::disconnectFromHost);
    p_ptr->checkAliveTimer->start();
}

CTcpSocket::~CTcpSocket()
{
    delete p_ptr;
}

void CTcpSocket::readPacket()
{
    p_ptr->checkAliveTimer->start();
    while (canReadLine()) {
        QByteArray data = readLine();
        data.chop(1);
        if (!data.isEmpty())
            emit newPacket(data);
    }
}

void CTcpSocket::writePacket(const QByteArray &data)
{
    p_ptr->keepAliveTimer->start();
    write(data);
    if (!data.endsWith('\n'))
        putChar('\n');
    flush();
}

void CTcpSocket::keepAlive()
{
    putChar('\n');
}
