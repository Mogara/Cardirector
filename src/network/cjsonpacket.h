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

#ifndef CJSONPACKET_H
#define CJSONPACKET_H

#include "cabstractpacket.h"

MCD_BEGIN_NAMESPACE

class MCD_EXPORT CJsonPacket : public CAbstractPacket
{
public:
    CJsonPacket(int command) : CAbstractPacket(command) {}

    bool parse(const QByteArray &data);
    QByteArray toByteArray() const;
};

MCD_END_NAMESPACE

#endif // CJSONPACKET_H
