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

#ifndef CSERVERPLAYER_H
#define CSERVERPLAYER_H

#include "cabstractplayer.h"

#include <QObject>
#include <QHostAddress>

MCD_BEGIN_NAMESPACE

class CTcpSocket;
class CAbstractPacketParser;
class CServerPlayerPrivate;

class MCD_EXPORT CServerPlayer : public CAbstractPlayer
{
    Q_OBJECT

public:
    CServerPlayer(CTcpSocket *socket, QObject *parent = 0);
    ~CServerPlayer();

    void setSocket(CTcpSocket *socket);

    void logout();
    void kick();
    QHostAddress ip() const;

    void request(int command, const QVariant &data = QVariant());
    void reply(int command, const QVariant &data = QVariant());
    void notify(int command, const QVariant &data = QVariant());

    QVariant waitForReply();

signals:
    void disconnected();

protected:
    static void CheckVersionCommand(QObject *player, const QVariant &data);
    static void LoginCommand(QObject *receiver, const QVariant &data);
    static void LogoutCommand(QObject *receiver, const QVariant &);
    static void SpeakCommand(QObject *receiver, const QVariant &data);

    void handleUnknownPacket(const QByteArray &packet);

private:
    void initCallbacks();

    C_DISABLE_COPY(CServerPlayer)
    C_DECLARE_PRIVATE(CServerPlayer)

    CServerPlayerPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif // CSERVERPLAYER_H
