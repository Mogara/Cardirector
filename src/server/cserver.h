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

#ifndef CSERVER_H
#define CSERVER_H

#include "cglobal.h"

#include <QObject>
#include <QHostAddress>

class CAbstractPacketParser;
class CTcpServer;
class CTcpSocket;
class CServerPlayer;
class CServerPrivate;

class MCD_EXPORT CServer : public QObject
{
    Q_OBJECT

public:
    CServer(QObject *parent = 0);
    ~CServer();

    bool listen(const QHostAddress &address = QHostAddress::Any, ushort port = 0);

    void setAcceptMultipleClientsBehindOneIp(bool enabled);
    bool acceptMultipleClientsBehindOneIp() const;

    CServerPlayer *findPlayer(uint id);
    QMap<uint, CServerPlayer *> players() const;

    void setPacketParser(CAbstractPacketParser *parser);
    CAbstractPacketParser *packetParser() const;

    void broadcastNotification(int command, const QVariant &data = QVariant(), CServerPlayer *except = NULL);

signals:
    void newPlayer(CServerPlayer *player);

protected:
    void handleNewConnection(CTcpSocket *client);

    void onPlayerDisconnected();
    void onPlayerStateChanged();

private:
    C_DISABLE_COPY(CServer)
    C_DECLARE_PRIVATE(CServer)
    CServerPrivate *p_ptr;
};

#endif
