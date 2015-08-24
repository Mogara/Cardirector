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

#ifndef CABSTRACTSERVERUSER_H
#define CABSTRACTSERVERUSER_H

#include "cabstractuser.h"

MCD_BEGIN_NAMESPACE

class CRoom;
class CServer;
class CServerUser;

class CAbstractServerUserPrivate;
class MCD_EXPORT CAbstractServerUser : public CAbstractUser
{
    Q_OBJECT

public:
    explicit CAbstractServerUser(CServer *server = 0);
    ~CAbstractServerUser();

    CServer *server() const;
    CRoom *room() const;
    void setRoom(CRoom *room);

    QVariant briefIntroduction() const;

    virtual bool controlledByClient() const = 0;

    CServerUser *toServerUser();

private:
    C_DISABLE_COPY(CAbstractServerUser)
    C_DECLARE_PRIVATE(CAbstractServerUser)
    CAbstractServerUserPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif // CABSTRACTSERVERUSER_H
