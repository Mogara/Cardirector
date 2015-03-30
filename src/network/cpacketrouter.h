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

#ifndef CPACKETROUTER_H
#define CPACKETROUTER_H

#include "cglobal.h"
#include "cpacket.h"
#include "cabstractpacketparser.h"

#include <QObject>
#include <QVariant>

MCD_BEGIN_NAMESPACE

template<typename T>
class MCD_EXPORT CPacketRouter : public QObject
{
public:
    typedef void (*Callback)(T *receiver, const QVariant &data);

    CPacketRouter(T *receiver, CAbstractPacketParser *parser)
        : QObject(receiver)
        , m_receiver(receiver)
        , m_parser(parser)
    {
    }

    void handlePacket(const QByteArray &rawPacket)
    {
        if (m_receiver == NULL)
            return;

        CPacket packet = m_parser->parse(rawPacket);
        if (!packet.isValid())
            return;

        if (packet.type() == CPacket::TYPE_NOTIFICATION) {
            Callback func = m_callbacks.value(packet.command());
            if (func)
                (*func)(m_receiver, packet.data());
        } else if (packet.type() == CPacket::TYPE_REQUEST) {
            Callback func = m_interactions.value(packet.command());
            if (func)
                (*func)(m_receiver, packet.data());
         }
    }

    void addInteraction(int command, Callback func)
    {
        m_interactions[command] = func;
    }

    void addCallback(int command, Callback func)
    {
        m_callbacks[command] = func;
    }

protected:
    QMap<int, Callback> m_interactions; //For requests
    QMap<int, Callback> m_callbacks;    //For notifications

    T *m_receiver;
    CAbstractPacketParser *m_parser;
};

MCD_END_NAMESPACE

#endif // CPACKETROUTER_H
