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

#include "cjsonpacketparser.h"

#include <QJsonDocument>

CPacket CJsonPacketParser::parse(const QByteArray &data)
{
    CPacket packet;
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull())
        return packet;

    QVariant json = doc.toVariant();
    if (!json.canConvert<QVariantList>())
        return packet;

    QVariantList list(json.toList());
    if (list.length() < 3)
        return packet;

    const QVariant &command = list.at(0);
    const QVariant &type = list.at(1);
    if (command.canConvert<int>() && type.canConvert<int>()) {
        packet.setCommand(command.toInt());
        packet.setType(static_cast<CPacket::Type>(type.toInt()));
        packet.setData(list.at(2));
    }

    return packet;
}

QByteArray CJsonPacketParser::parse(const CPacket &packet)
{
    QVariantList message;
    message << packet.command();
    message << packet.type();
    message << packet.data();

    QJsonDocument doc = QJsonDocument::fromVariant(message);
    return doc.toJson(QJsonDocument::Compact);
}
