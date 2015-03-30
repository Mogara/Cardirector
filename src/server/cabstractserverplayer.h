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

#ifndef CABSTRACTSERVERPLAYER_H
#define CABSTRACTSERVERPLAYER_H

#include "cabstractplayer.h"

#include <QObject>

MCD_BEGIN_NAMESPACE

class CTcpSocket;
class CAbstractServerPlayerPrivate;

class CAbstractServerPlayer : public CAbstractPlayer
{
    Q_OBJECT

public:
    explicit CAbstractServerPlayer(QObject *parent = 0);
    CAbstractServerPlayer(CTcpSocket *socket, QObject *parent = 0);
    ~CAbstractServerPlayer();

    void setSocket(CTcpSocket *socket);

signals:
    void disconnected();

protected:
    void handleNewPacket(const QByteArray &rawPacket);

private:
    C_DISABLE_COPY(CAbstractServerPlayer)
    C_DECLARE_PRIVATE(CAbstractServerPlayer)

    CAbstractServerPlayerPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif // CABSTRACTSERVERPLAYER_H
