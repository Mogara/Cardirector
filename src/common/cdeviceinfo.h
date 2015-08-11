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

#ifndef CDEVICEINFO_H
#define CDEVICEINFO_H

#include "cglobal.h"

#include <QObject>
#include <QGuiApplication>
#include <QScreen>

MCD_BEGIN_NAMESPACE

class MCD_EXPORT CDeviceInfo : public QObject
{
    Q_OBJECT
public:
    explicit CDeviceInfo(QObject *parent = 0);
    ~CDeviceInfo();

    Q_INVOKABLE int gu(const QVariant &point)
    {
        double dots = point.toDouble();
        const int standardDpi = 96;
        static int dpi = qApp->primaryScreen()->logicalDotsPerInch();

        return dots * dpi / standardDpi;
    }
};

MCD_END_NAMESPACE

#endif // CDEVICEINFO_H
