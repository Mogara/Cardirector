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

#ifndef CPACKET_H
#define CPACKET_H

#include "cglobal.h"

#include <QVariant>

class CPacketPrivate;

class MCD_EXPORT CPacket
{
public:
    enum Type{
        TYPE_NOTIFICATION,
        TYPE_REQUEST,
        TYPE_REPLY
    };

    CPacket();
    CPacket(int command, Type type);
    CPacket(const CPacket &source);
    ~CPacket();

    bool isValid() const { return d != Q_NULLPTR; }

    int command() const;
    void setCommand(int command);

    Type type() const;
    void setType(Type type);

    void setData(const QVariant &data);
    const QVariant &data() const;

protected:
    void detach();
    CPacketPrivate *d;
};

#endif // CPACKET_H
