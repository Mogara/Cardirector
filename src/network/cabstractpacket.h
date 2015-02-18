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

#ifndef CABSTRACTPACKET_H
#define CABSTRACTPACKET_H

#include "cglobal.h"

#include <QVariant>

MCD_BEGIN_NAMESPACE

class CAbstractPacketPrivate;

class MCD_EXPORT CAbstractPacket
{
public:
    CAbstractPacket(int command) : m_command(command) {}

    int command() const { return m_command; }

    void setData(const QVariant &data) { m_data = data; }
    const QVariant &data() const { return m_data; }

    virtual bool parse(const QByteArray &data) = 0;
    virtual QByteArray toByteArray() const = 0;

protected:
    int m_command;
    QVariant m_data;
};

MCD_END_NAMESPACE

#endif // CABSTRACTPACKET_H
