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

#include "crobot.h"
#include "croom.h"
#include "cserver.h"

//class CRobotPrivate;

CRobot::CRobot(CRoom *room)
    : CAbstractServerUser(room->server())
{
    room->addRobot(this);
    setId(server()->newUserId());
    setScreenName(room->newRobotName());
}

CRobot::~CRobot()
{

}

