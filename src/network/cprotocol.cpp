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

#include "cprotocol.h"

static QMap<int, QString> CCommandString;

void cRegisterCommand(int command, const QString &name)
{
    CCommandString.insert(command, name);
}

class CCommandAdder{
public:
    CCommandAdder()
    {
        C_REGISTER_COMMAND(UNKNOWN);
        C_REGISTER_COMMAND(CHECK_VERSION);
        C_REGISTER_COMMAND(SIGNUP);
        C_REGISTER_COMMAND(LOGIN);
        C_REGISTER_COMMAND(LOGOUT);
        C_REGISTER_COMMAND(ERROR);
        C_REGISTER_COMMAND(SPEAK);
        C_REGISTER_COMMAND(SET_USER_LIST);
        C_REGISTER_COMMAND(ADD_USER);
        C_REGISTER_COMMAND(REMOVE_USER);
        C_REGISTER_COMMAND(SET_ROOM_LIST);
        C_REGISTER_COMMAND(CREATE_ROOM);
        C_REGISTER_COMMAND(ENTER_ROOM);
        C_REGISTER_COMMAND(START_GAME);
        C_REGISTER_COMMAND(ADD_ROBOT);
        C_REGISTER_COMMAND(REMOVE_ROBOT);
    }
};
static CCommandAdder adder;
