/********************************************************************
    Copyright (c) 2013-2016 - Mogara

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

#include "cnetwork.h"

#include <QSettings>
#include <QCoreApplication>

void cRegisterUrlScheme(const QString &introduction)
{
#ifdef Q_OS_WIN
    QString scheme = qApp->applicationName();
    scheme.remove(QRegularExpression(QStringLiteral("[^A-Za-z]")));

    QString appPath = QCoreApplication::applicationFilePath();
    appPath.replace(QLatin1Char('/'), QLatin1Char('\\'));

    QSettings reg(QStringLiteral("HKEY_CLASSES_ROOT"), QSettings::NativeFormat);
    reg.beginGroup(scheme);
    reg.setValue(QStringLiteral("."), introduction);
    reg.setValue(QStringLiteral("URL Protocol"), QStringLiteral(""));
    reg.setValue(QStringLiteral("DefaultIcon/."), QStringLiteral("%1, 0").arg(appPath));
    reg.setValue(QStringLiteral("Shell/Open/Command/."), QStringLiteral("\"%1\"").arg(appPath) + QStringLiteral(" \"%1\""));
    reg.endGroup();
#else
    C_UNUSED(introduction);
#endif
}
