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

#include "cqmlapplicationengine.h"

static const struct {
    const char *type;
        int major, minor;
    } qmldir [] = {
        { "MainWindow", 1, 0 }
};

CQmlApplicationEngine::CQmlApplicationEngine()
{
    const QString filesLocation = fileLocation();
    for (int i = 0; i < int(sizeof(qmldir)/sizeof(qmldir[0])); i++)
        qmlRegisterType(QUrl(filesLocation + "/" + qmldir[i].type + ".qml"), "Cardirector.Gui", qmldir[i].major, qmldir[i].minor, qmldir[i].type);

    if (isLoadedFromResource())
        addImportPath(QStringLiteral("qrc:/"));
}

CQmlApplicationEngine::~CQmlApplicationEngine()
{

}

QString CQmlApplicationEngine::fileLocation() const
{
    if (isLoadedFromResource())
        return "qrc:/Cardirector/Gui";
    return baseUrl().toString();
}

bool CQmlApplicationEngine::isLoadedFromResource() const
{
    // If one file is missing, it will load all the files from the resource
    QFile file(baseUrl().toLocalFile() + "/MainWindow.qml");
    if (!file.exists())
        return true;
    return false;
}

