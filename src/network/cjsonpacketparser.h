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

#ifndef CJSONPACKETPARSER_H
#define CJSONPACKETPARSER_H

#include "cglobal.h"
#include "cabstractpacketparser.h"

MCD_BEGIN_NAMESPACE

class MCD_EXPORT CJsonPacketParser : public CAbstractPacketParser
{
public:
    CPacket parse(const QByteArray &data);
    QByteArray parse(const CPacket &packet);
};

MCD_END_NAMESPACE

#endif // CJSONPACKETPARSER_H

