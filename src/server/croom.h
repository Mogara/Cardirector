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

#ifndef CROOM_H
#define CROOM_H

#include "cglobal.h"

#include <QObject>

class CAbstractGameLogic;
class CServer;
class CServerPlayer;

class CRoomPrivate;
class CRoom : public QObject
{
public:
    CRoom(CServer *server);
    ~CRoom();

    void setGameLogic(CAbstractGameLogic *gameLogic);
    CAbstractGameLogic *gameLogic() const;

    void addPlayer(CServerPlayer *player);
    void removePlayer(CServerPlayer *player);
    QMap<uint, CServerPlayer *> players() const;
    CServerPlayer *findPlayer(int id) const;

    void broadcastNotification(int command, const QVariant &data, CServerPlayer *except = NULL);

protected:
    //Slots for CServerPlayer. Do not call them directly.
    void onPlayerSpeaking(const QString &message);
    void onPlayerDisconnected();

private:
    C_DISABLE_COPY(CRoom)
    C_DECLARE_PRIVATE(CRoom)
    CRoomPrivate *p_ptr;
};

#endif // CROOM_H
