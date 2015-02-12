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

#ifndef CTCPSERVER_H
#define CTCPSERVER_H

#include "cglobal.h"

#include <QTcpServer>

class CTcpSocket;

class MCD_EXPORT CTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    CTcpServer(QObject *parent = 0);

signals:
    void newSocket(CTcpSocket *socket);

protected:
    void incomingConnection(qintptr handle);

private:
    void newConnection() {}
    QTcpSocket *nextPendingConnection() {return NULL;}
};

#endif // CTCPSERVER_H
