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

#ifndef CTCPSOCKET_H
#define CTCPSOCKET_H

#include "cglobal.h"

#include <QTcpSocket>
#include <QByteArray>
#include <QHostAddress>

MCD_BEGIN_NAMESPACE

class CTcpSocketPrivate;

class MCD_EXPORT CTcpSocket : public QTcpSocket
{
    Q_OBJECT

public:
    explicit CTcpSocket(QObject *parent = 0);
    ~CTcpSocket();

    void writePacket(const QByteArray &data);

signals:
    void newPacket(const QByteArray &data);

protected:
    void readPacket();
    void keepAlive();

private:
    C_DISABLE_COPY(CTcpSocket)
    C_DECLARE_PRIVATE(CTcpSocket)
    CTcpSocketPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif // CTCPSOCKET_H
