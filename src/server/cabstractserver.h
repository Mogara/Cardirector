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

#ifndef CABSTRACTSERVER_H
#define CABSTRACTSERVER_H

#include "cglobal.h"

#include <QObject>
#include <QHostAddress>

class CTcpServer;
class CTcpSocket;
class CAbstractServerPlayer;
class CAbstractServerPrivate;

class MCD_EXPORT CAbstractServer : public QObject
{
    Q_OBJECT

public:
    CAbstractServer(QObject *parent = 0);
    ~CAbstractServer();

    bool listen(const QHostAddress &address = QHostAddress::Any, ushort port = 0);

    void setAcceptMultipleClientsBehindOneIp(bool enabled);
    bool acceptMultipleClientsBehindOneIp() const;

    const QList<CAbstractServerPlayer *> &players() const;

signals:
    void newPlayer(CAbstractServerPlayer *player);

protected:
    virtual CAbstractServerPlayer *createPlayer(CTcpSocket *client);
    void handleNewConnection(CTcpSocket *client);

private:
    C_DISABLE_COPY(CAbstractServer)
    C_DECLARE_PRIVATE(CAbstractServer)
    CAbstractServerPrivate *p_ptr;
};

#endif
