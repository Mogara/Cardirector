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

#ifndef CROOMSETTINGS_H
#define CROOMSETTINGS_H

#include "cglobal.h"

MCD_BEGIN_NAMESPACE

class MCD_EXPORT CRoomSettings
{
    Q_GADGET

    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(int capacity MEMBER capacity)
    Q_PROPERTY(int timeout MEMBER timeout)

    virtual const QMetaObject *metaObject() const;

public:
    CRoomSettings();

    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key);

    QVariant toVariant() const;

    QString name;
    int capacity;
    int timeout;  //seconds
};

MCD_END_NAMESPACE

#endif // CROOMSETTINGS_H
