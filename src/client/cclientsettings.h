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

#include <QSettings>

MCD_BEGIN_NAMESPACE

class CClientSettingsPrivate;

class MCD_EXPORT CClientSettings : public QSettings
{
    Q_OBJECT

public:
    Q_PROPERTY(QString locale READ locale WRITE setLocale NOTIFY localeChanged)

    CClientSettings(const QString &organization, const QString &application = QString(), QObject *parent = 0);
    CClientSettings(Scope scope, const QString &organization, const QString & application = QString(), QObject *parent = 0);
    CClientSettings(Format format, Scope scope, const QString &organization, const QString &application = QString(), QObject *parent = 0);
    CClientSettings(const QString &fileName, Format format, QObject *parent = 0);
    CClientSettings(QObject *parent = 0);
    virtual ~CClientSettings();

    QString locale() const;
    void setLocale(const QString &locale);

signals:
    void localeChanged();

private:
    C_DISABLE_COPY(CClientSettings)
    C_DECLARE_PRIVATE(CClientSettings)
    CClientSettingsPrivate *p_ptr;

    void init();
};

MCD_END_NAMESPACE

#endif // CCLIENTSETTINGS_H
