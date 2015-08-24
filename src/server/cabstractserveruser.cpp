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

#include "cabstractserveruser.h"
#include "crobot.h"
#include "cserver.h"
#include "cserveruser.h"

class CAbstractServerUserPrivate
{
public:
    CServer *server;
    CRoom *room;
};

CAbstractServerUser::CAbstractServerUser(CServer *server)
    : CAbstractUser(server)
{
    p_ptr = new CAbstractServerUserPrivate;

    p_ptr->server = server;
    p_ptr->room = NULL;
}

CAbstractServerUser::~CAbstractServerUser()
{
    delete p_ptr;
}

CServer *CAbstractServerUser::server() const
{
    return p_ptr->server;
}

CRoom *CAbstractServerUser::room() const
{
    return p_ptr->room;
}

void CAbstractServerUser::setRoom(CRoom *room)
{
    p_ptr->room = room;
}

QVariant CAbstractServerUser::briefIntroduction() const
{
    QVariantList arguments;
    arguments << id();
    arguments << screenName();
    arguments << avatar();
    return arguments;
}

CServerUser *CAbstractServerUser::toServerUser()
{
    return qobject_cast<CServerUser *>(this);
}

CRobot *CAbstractServerUser::toRobot()
{
    return qobject_cast<CRobot *>(this);
}

