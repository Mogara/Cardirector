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

#include "cserveragent.h"
#include "cserverrobot.h"
#include "cserver.h"
#include "cserveruser.h"

class CServerAgentPrivate
{
public:
    CServer *server;
    CRoom *room;
};

CServerAgent::CServerAgent(CServer *server)
    : CAbstractUser(server)
{
    p_ptr = new CServerAgentPrivate;

    p_ptr->server = server;
    p_ptr->room = NULL;
}

CServerAgent::~CServerAgent()
{
    delete p_ptr;
}

CServer *CServerAgent::server() const
{
    return p_ptr->server;
}

CRoom *CServerAgent::room() const
{
    return p_ptr->room;
}

void CServerAgent::setRoom(CRoom *room)
{
    p_ptr->room = room;
}

QVariant CServerAgent::briefIntroduction() const
{
    QVariantList arguments;
    arguments << id();
    arguments << screenName();
    arguments << avatar();
    return arguments;
}

CServerUser *CServerAgent::toServerUser()
{
    return qobject_cast<CServerUser *>(this);
}

CServerRobot *CServerAgent::toRobot()
{
    return qobject_cast<CServerRobot *>(this);
}

