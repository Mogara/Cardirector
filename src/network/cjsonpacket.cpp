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

#include "cjsonpacket.h"

#include <QJsonDocument>

bool CJsonPacket::parse(const QByteArray &data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QVariantList packet = doc.toVariant().toList();
    if (packet.size() != 2)
        return false;
    m_command = packet.at(0).toInt();
    m_data = packet.at(1);
    return true;
}

QByteArray CJsonPacket::toByteArray() const
{
    QVariantList message;
    message << command();
    message << data();

    QJsonDocument doc = QJsonDocument::fromVariant(message);
    if (doc.isEmpty())
        return QByteArray();
    return doc.toJson(QJsonDocument::Compact);
}
