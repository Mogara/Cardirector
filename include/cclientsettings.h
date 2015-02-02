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

#ifndef CCLIENTSETTINGS_H
#define CCLIENTSETTINGS_H

#include "cglobal.h"

#include <QSize>
#include <QPoint>
#include <QSettings>

MCD_BEGIN_NAMESPACE

class CClientSettingsPrivate;

class MCD_EXPORT CClientSettings : public QSettings
{
    Q_OBJECT

public:
    Q_PROPERTY(QSize windowSize READ windowSize WRITE setWindowSize NOTIFY windowSizeChanged)
    Q_PROPERTY(QPoint windowPos READ windowPos WRITE setWindowPos NOTIFY windowPosChanged)
    Q_PROPERTY(Qt::WindowState windowState READ windowState WRITE setWindowState NOTIFY windowStateChanged)

    CClientSettings(const QString &organization, const QString &application = QString(), QObject *parent = 0);
    CClientSettings(Scope scope, const QString &organization, const QString & application = QString(), QObject *parent = 0);
    CClientSettings(Format format, Scope scope, const QString &organization, const QString &application = QString(), QObject *parent = 0);
    CClientSettings(const QString &fileName, Format format, QObject *parent = 0);
    CClientSettings(QObject *parent = 0);
    virtual ~CClientSettings();

    Q_INVOKABLE QSize windowSize() const;
    void setWindowSize(const QSize &size);

    Q_INVOKABLE QPoint windowPos() const;
    void setWindowPos(const QPoint &pos);

    Q_INVOKABLE Qt::WindowState windowState() const;
    void setWindowState(const Qt::WindowState state);

signals:
    void windowSizeChanged();
    void windowPosChanged();
    void windowStateChanged();

private:
    C_DISABLE_COPY(CClientSettings)
    C_DECLARE_PRIVATE(CClientSettings)
    CClientSettingsPrivate *p_ptr;

    void init();
};

MCD_END_NAMESPACE

#endif // CCLIENTSETTINGS_H
