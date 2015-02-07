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

#include "cguiplugin.h"

#include <QFile>
#include <QtQml>

static void initResources()
{
    Q_INIT_RESOURCE(gui);
}

MCD_BEGIN_NAMESPACE

static const struct {
    const char *type;
    int major, minor;
} qmldir [] = {
    { "MainWindow", 1, 0 }
};

void CGuiPlugin::registerTypes(const char *uri)
{
    // @uri Cardirector.Gui
    Q_ASSERT(uri == QLatin1String("Cardirector.Gui"));

    //qmlRegisterType<CGuiPlugin>(uri, 1, 0, "CGuiPlugin");
    const QString filesLocation = fileLocation();
    for (int i = 0; i < int(sizeof(qmldir)/sizeof(qmldir[0])); i++)
        qmlRegisterType(QUrl(filesLocation + "/" + qmldir[i].type + ".qml"), uri, qmldir[i].major, qmldir[i].minor, qmldir[i].type);
}

void CGuiPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    C_UNUSED(uri)

    if (isLoadedFromResource())
        engine->addImportPath(QStringLiteral("qrc:/"));
}

QString CGuiPlugin::fileLocation() const
{
    if (isLoadedFromResource())
        return "qrc:/Cardirector/Gui";
    return baseUrl().toString();
}

bool CGuiPlugin::isLoadedFromResource() const
{
    // If one file is missing, it will load all the files from the resource
    QFile file(baseUrl().toLocalFile() + "/MainWindow.qml");
    if (!file.exists())
        return true;
    return false;
}

MCD_END_NAMESPACE


