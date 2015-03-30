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

class MCD_EXPORT CAbstractServer : public QObject
{
public:
    CAbstractServer(QObject *parent = 0);

    void setAcceptMultipleClientsBehindOneIp(bool enabled) { m_acceptMultipleClientsBehindOneIp = enabled; }
    bool acceptMultipleClientsBehindOneIp() const { return m_acceptMultipleClientsBehindOneIp; }

protected:
    void handleNewConnection(CTcpSocket *client);

    bool m_acceptMultipleClientsBehindOneIp;
    QSet<QHostAddress> m_clientIp;
    CTcpServer *m_server;
};

#endif
