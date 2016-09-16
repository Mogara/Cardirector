/********************************************************************
    Copyright (c) 2013-2015 - Mogara

    This file is part of QSanguosha.

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

#include "croomsettings.h"

#include <QMetaProperty>

MCD_BEGIN_NAMESPACE

CRoomSettings::CRoomSettings()
    : capacity(0)
    , timeout(15)
{
}

CRoomSettings::~CRoomSettings()
{

}

const QMetaObject *CRoomSettings::metaObject() const
{
    return &staticMetaObject;
}

void CRoomSettings::setValue(const QString &key, const QVariant &value)
{
    const QMetaObject *meta = metaObject();
    int index = meta->indexOfProperty(key.toLatin1());
    QMetaProperty property = meta->property(index);
    if (property.isWritable())
        property.writeOnGadget(this, value);
}

QVariant CRoomSettings::value(const QString &key)
{
    const QMetaObject *meta = metaObject();
    int index = meta->indexOfProperty(key.toLatin1());
    QMetaProperty property = meta->property(index);
    if (property.isReadable())
        return property.readOnGadget(this);
    return QVariant();
}

QVariant CRoomSettings::toVariant() const
{
    const QMetaObject *meta = metaObject();
    QVariantMap data;
    int max = meta->propertyCount();
    for (int i = 0; i < max; i++) {
        QMetaProperty property = meta->property(i);
        if (property.isReadable())
            data[QString::fromUtf8(property.name())] = property.readOnGadget(this);
    }
    return data;
}

MCD_END_NAMESPACE
