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

#include "cabstractgamelogic.h"
#include "cabstractplayer.h"
#include "cserveruser.h"
#include "crobot.h"
#include "croom.h"

class CAbstractGameLogicPrivate
{
public:
    CRoom *room;
    QMap<uint, CAbstractPlayer *> players;
    QMap<CAbstractPlayer *, CAbstractServerUser *> users;
};

CAbstractGameLogic::CAbstractGameLogic(CRoom *parent)
    : QThread(parent)
    , p_ptr(new CAbstractGameLogicPrivate)
{
    p_ptr->room = parent;
}

CAbstractGameLogic::~CAbstractGameLogic()
{
    delete p_ptr;
}

CRoom *CAbstractGameLogic::room() const
{
    return p_ptr->room;
}

CAbstractServerUser *CAbstractGameLogic::findAbstractUser(CAbstractPlayer *player)
{
    return p_ptr->users.value(player);
}

void CAbstractGameLogic::start(Priority priority)
{
    int playerId = 0;

    foreach (CServerUser *user, p_ptr->room->users()) {
        CAbstractPlayer *player = createPlayer(user);
        player->setId(++playerId);

        p_ptr->players.insert(player->id(), player);
        p_ptr->users.insert(player, user);
    }

    foreach (CRobot *robot, p_ptr->room->robots()) {
        CAbstractPlayer *player = createPlayer(robot);
        player->setId(++playerId);

        p_ptr->players.insert(player->id(), player);
        p_ptr->users.insert(player, robot);
    }

    QThread::start(priority);
}
