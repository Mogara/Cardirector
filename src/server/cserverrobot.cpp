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

#include "croom.h"
#include "cserver.h"
#include "cserverrobot.h"

//class CServerRobotPrivate;

CServerRobot::CServerRobot(CRoom *room)
    : CServerAgent(room->server())
{
    static uint robotId = 0;
    robotId++;
    setId(robotId);
    setScreenName(room->newRobotName());

    room->addRobot(this);
}

CServerRobot::~CServerRobot()
{

}

//@to-do: implement all interactive functions below
void CServerRobot::request(int command, const QVariant &data, int timeout)
{
    C_UNUSED(command);
    C_UNUSED(data);
    C_UNUSED(timeout);
}

void CServerRobot::reply(int command, const QVariant &data)
{
    C_UNUSED(command);
    C_UNUSED(data);
}

void CServerRobot::notify(int command, const QVariant &data)
{
    C_UNUSED(command);
    C_UNUSED(data);
}

void CServerRobot::executeRequest(int timeout)
{
    C_UNUSED(timeout);
}

void CServerRobot::cancelRequest()
{

}

QVariant CServerRobot::waitForReply()
{
    return QVariant();
}

QVariant CServerRobot::waitForReply(int timeout)
{
    C_UNUSED(timeout);
    return QVariant();
}

