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
#include "cserverrobot.h"
#include "croom.h"

class CAbstractGameLogicPrivate
{
public:
    CRoom *room;
    QMap<uint, CAbstractPlayer *> players;
    QMap<CAbstractPlayer *, CServerAgent *> player2agent;
    QMap<CServerAgent *, CAbstractPlayer *> agent2player;
    bool isRunning;
};

CAbstractGameLogic::CAbstractGameLogic(CRoom *room)
    : p_ptr(new CAbstractGameLogicPrivate)
{
    p_ptr->room = room;
    p_ptr->isRunning = false;
}

CAbstractGameLogic::~CAbstractGameLogic()
{
    delete p_ptr;
}

CRoom *CAbstractGameLogic::room() const
{
    return p_ptr->room;
}

QMap<uint, CAbstractPlayer *> CAbstractGameLogic::abstractPlayers() const
{
    return p_ptr->players;
}

CAbstractPlayer *CAbstractGameLogic::findAbstractPlayer(uint id) const
{
    return p_ptr->players.value(id);
}

CAbstractPlayer *CAbstractGameLogic::findAbstractPlayer(CServerAgent *agent) const
{
    return p_ptr->agent2player.value(agent);
}

CServerAgent *CAbstractGameLogic::findAgent(CAbstractPlayer *player) const
{
    return p_ptr->player2agent.value(player);
}

CServerUser *CAbstractGameLogic::findUser(CAbstractPlayer *player) const
{
    return qobject_cast<CServerUser *>(p_ptr->player2agent.value(player));
}

CServerRobot *CAbstractGameLogic::findRobot(CAbstractPlayer *player) const
{
    return qobject_cast<CServerRobot *>(p_ptr->player2agent.value(player));
}

void CAbstractGameLogic::start()
{
    int playerId = 0;

    foreach (CServerUser *user, p_ptr->room->users()) {
        CAbstractPlayer *player = createPlayer(user);
        player->setId(++playerId);

        p_ptr->players.insert(player->id(), player);
        p_ptr->player2agent.insert(player, user);
        p_ptr->agent2player.insert(user, player);
    }

    foreach (CServerRobot *robot, p_ptr->room->robots()) {
        CAbstractPlayer *player = createPlayer(robot);
        player->setId(++playerId);

        p_ptr->players.insert(player->id(), player);
        p_ptr->player2agent.insert(player, robot);
        p_ptr->agent2player.insert(robot, player);
    }

    p_ptr->isRunning = true;
    emit started();
    run();
    p_ptr->isRunning = false;
    emit finished();
}

bool CAbstractGameLogic::isRunning() const
{
    return p_ptr->isRunning;
}
