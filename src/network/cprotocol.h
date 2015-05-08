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

#ifndef CPROTOCOL_H
#define CPROTOCOL_H

#include "cglobal.h"

#include <QString>
#include <QMap>

MCD_BEGIN_NAMESPACE

enum CCommandType {
    S_COMMAND_UNKNOWN,
    S_COMMAND_CHECK_VERSION,
    S_COMMAND_SIGNUP,
    S_COMMAND_LOGIN,
    S_COMMAND_LOGOUT,
    S_COMMAND_ERROR,
    S_COMMAND_SPEAK,
    S_COMMAND_SET_PLAYER_LIST,
    S_COMMAND_ADD_PLAYER,
    S_COMMAND_REMOVE_PLAYER,

    CARDIRECTOR_SYSTEM_COMMAND_COUNT
};

extern QMap<CCommandType, QString> CCommandString; //Permanently saved commands should be converted to string instead of integer

MCD_END_NAMESPACE

#define C_REGISTER_COMMAND(command) CCommandString.insert(S_COMMAND_##command, #command)

#endif // CPROTOCOL_H