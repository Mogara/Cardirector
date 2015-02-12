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

#include "cabstractpacket.h"

class CAbstractPacketPrivate
{
public:
    int command;
    QVariant data;
};

CAbstractPacket::CAbstractPacket(int command)
    : p_ptr(new CAbstractPacketPrivate)
{
    p_ptr->command = command;
}

CAbstractPacket::~CAbstractPacket()
{
    delete p_ptr;
}

int CAbstractPacket::command() const
{
    return p_ptr->command;
}

void CAbstractPacket::setData(const QVariant &data)
{
    p_ptr->data = data;
}

const QVariant &CAbstractPacket::data() const
{
    return p_ptr->data;
}
