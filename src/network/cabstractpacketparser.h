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

#ifndef CABSTRACTPACKETPARSER_H
#define CABSTRACTPACKETPARSER_H

#include "cpacket.h"

MCD_BEGIN_NAMESPACE

class MCD_EXPORT CAbstractPacketParser
{
public:
    virtual ~CAbstractPacketParser() {}
    virtual CPacket parse(const QByteArray &data) = 0;
    virtual QByteArray parse(const CPacket &packet) = 0;
};

MCD_END_NAMESPACE

#endif // CABSTRACTPACKETPARSER_H
