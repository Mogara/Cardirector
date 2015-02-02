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

    QSize windowSize;
    QPoint windowPos;
    Qt::WindowState windowState;
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

CClientSettings::~CClientSettings()
{

}

QSize CClientSettings::windowSize() const
{
    return p_ptr->windowSize;
}

void CClientSettings::setWindowSize(const QSize &size)
{
    if (p_ptr->windowSize != size) {
        p_ptr->windowSize = size;
        setValue("window/size", size);
        emit windowSizeChanged();
    }
}

QPoint CClientSettings::windowPos() const
{
    return p_ptr->windowPos;
}

void CClientSettings::setWindowPos(const QPoint &pos)
{
    if (p_ptr->windowPos != pos) {
        p_ptr->windowPos = pos;
        setValue("window/pos", pos);
        emit windowPosChanged();
    }
}

Qt::WindowState CClientSettings::windowState() const
{
    return p_ptr->windowState;
}

void CClientSettings::setWindowState(const Qt::WindowState state)
{
    if (p_ptr->windowState != state) {
        p_ptr->windowState = state;
        setValue("window/state", (int)state);
        emit windowStateChanged();
    }
}

void CClientSettings::init()
{
    p_ptr = new CClientSettingsPrivate;

    beginGroup("window");
    p_ptr->windowSize = value("size").toSize();
    p_ptr->windowPos = value("pos").toPoint();
    p_ptr->windowState = (Qt::WindowState)value("state").toInt();
    endGroup();
}

