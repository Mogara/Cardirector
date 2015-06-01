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

#ifndef CPACKETROUTER_H
#define CPACKETROUTER_H

#include "cglobal.h"
#include "cabstractpacketparser.h"

#include <QObject>
#include <QVariant>

MCD_BEGIN_NAMESPACE

class CTcpSocket;
class CPacketRouterPrivate;

class MCD_EXPORT CPacketRouter : public QObject
{
    Q_OBJECT

public:
    typedef void (*Callback)(QObject *receiver, const QVariant &data);

    CPacketRouter(QObject *receiver, CTcpSocket *socket, CAbstractPacketParser *parser);
    ~CPacketRouter();

    CTcpSocket *socket();
    CAbstractPacketParser *parser();

    void setSocket(CTcpSocket *socket);

    void setInteractions(const QHash<int, Callback> *interactions);
    void setCallbacks(const QHash<int, Callback> *callbacks);

    void request(int command, const QVariant &data = QVariant(), int timeout = -1);
    void reply(int command, const QVariant &data = QVariant());
    void notify(int command, const QVariant &data = QVariant());

    int requestTimeout() const;

    QVariant waitForReply();
    QVariant waitForReply(int timeout);

signals:
    void messageReady(const QByteArray &message);
    void unknownPacket(const QByteArray &packet);

protected:
    void handlePacket(const QByteArray &rawPacket);

private:
    C_DISABLE_COPY(CPacketRouter)
    C_DECLARE_PRIVATE(CPacketRouter)
    CPacketRouterPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif // CPACKETROUTER_H
