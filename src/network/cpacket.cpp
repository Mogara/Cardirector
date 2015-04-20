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

#include "cpacket.h"

#include <QAtomicInt>

class CPacketPrivate
{
public:
    int command;
    CPacket::Type type;
    QVariant data;
    QAtomicInt ref;

    CPacketPrivate()
        : ref(1)
    {
    }

    CPacketPrivate *clone()
    {
        CPacketPrivate *packet = new CPacketPrivate;
        packet->command = command;
        packet->type = type;
        packet->data = data;
        return packet;
    }
};

CPacket::CPacket()
    : d(NULL)
{
}

CPacket::CPacket(int command, Type type)
    : d(new CPacketPrivate)
{
    d->command = command;
    d->type = type;
}

CPacket::~CPacket()
{
    if (d != NULL) {
        if (d->ref.load() == 1)
            delete d;
        else
            d->ref.deref();
    }
}

CPacket::CPacket(const CPacket &source)
    : d(source.d)
{
    if (d != NULL)
        d->ref.ref();
}

int CPacket::command() const
{
    return d->command;
}

void CPacket::setCommand(int command)
{
    detach();
    d->command = command;
}

CPacket::Type CPacket::type() const
{
    return d->type;
}

void CPacket::setType(CPacket::Type type)
{
    detach();
    d->type = type;
}

const QVariant &CPacket::data() const
{
    return d->data;
}

void CPacket::setData(const QVariant &data)
{
    detach();
    d->data = data;
}

void CPacket::detach()
{
    if (d) {
        if (d->ref.load() > 1) {
            d->ref.deref();
            d = d->clone();
        }
    } else {
        d = new CPacketPrivate;
    }
}
