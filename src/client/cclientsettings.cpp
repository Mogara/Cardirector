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

#include "cpch.h"
#include "cclientsettings.h"

class CClientSettingsPrivate
{
public:
    CClientSettingsPrivate()
    {

    }
};

CClientSettings::CClientSettings(const QString &organization, const QString &application, QObject *parent)
    : QSettings(organization, application, parent)
{
    init();
}

CClientSettings::CClientSettings(QSettings::Scope scope, const QString &organization, const QString &application, QObject *parent)
    : QSettings(scope, organization, application, parent)
{
    init();
}

CClientSettings::CClientSettings(QSettings::Format format, QSettings::Scope scope, const QString &organization, const QString &application, QObject *parent)
    : QSettings(format, scope, organization, application, parent)
{
    init();
}

CClientSettings::CClientSettings(const QString &fileName, QSettings::Format format, QObject *parent)
    : QSettings(fileName, format, parent)
{
    init();
}

CClientSettings::CClientSettings(QObject *parent)
    : QSettings(parent)
{
    init();
}

void CClientSettings::init()
{
    p_ptr = new CClientSettingsPrivate;
}

CClientSettings::~CClientSettings()
{
    delete p_ptr;
}
