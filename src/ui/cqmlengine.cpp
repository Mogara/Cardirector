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

#include "cqmlengine.h"
#include "cclientsettings.h"
#include "cdeviceinfo.h"
#include "cimageprovider.h"
#include "csound.h"

#include <QtQml>
#include <QFile>

namespace
{
    struct QmlDirStruct
    {
        const char *type;
        int major, minor;
        bool isPublic;
    };

    const QmlDirStruct qmldir[] = {
        {"Dialog", 1, 0, true},
        {"FitInView", 1, 0, true},
        {"LogEdit", 1, 0, true},
        {"MetroButton", 1, 0, true},
        {"Splash", 1, 0, true},
        {"TileButton", 1, 0, true},
        {"ToolTipArea", 1, 0, true},
        {"ToolTip", 1, 0, false},
    };
}

class CQmlEnginePrivate
{
public:

    CQmlEnginePrivate()
    {

    }

    CQmlEngine *b_ptr;
    C_DECLARE_PUBLIC(CQmlEngine)

    QString fileLocation() const
    {
        if (isLoadedFromResource())
            return QStringLiteral("qrc:/Cardirector/Gui");
        return b_ptr->baseUrl().toString();
    }

    bool isLoadedFromResource() const
    {
        // If one file is missing, it will load all the files from the resource
        QFile file(b_ptr->baseUrl().toLocalFile() + QStringLiteral("/MetroButton.qml"));
        if (!file.exists())
            return true;
        return false;
    }
};

namespace
{
    QObject *cDeviceInfoProvider(QQmlEngine *, QJSEngine *)
    {
        CDeviceInfo *info = new CDeviceInfo;
        return info;
    }
}

CQmlEngine::CQmlEngine(QObject *p)
    : QQmlEngine(p), p_ptr(new CQmlEnginePrivate)
{
    p_ptr->b_ptr = this;

    const QString filesLocation = p_ptr->fileLocation();
    for (int i = 0; i < int(sizeof(qmldir)/sizeof(qmldir[0])); i++) {
        const char *uri = qmldir[i].isPublic ? "Cardirector.Gui" : "Cardirector.Gui.Private";
        qmlRegisterType(QUrl(filesLocation + QStringLiteral("/") + QString::fromUtf8(qmldir[i].type) + QStringLiteral(".qml")),
                        uri, qmldir[i].major, qmldir[i].minor, qmldir[i].type);
    }

    qmlRegisterType<CClientSettings>("Cardirector.Client", 1, 0, "ClientSettings");
    qmlRegisterType<CImageProvider>("Cardirector.Resource", 1, 0, "ImageProvider");
    qmlRegisterType<CSound>("Cardirector.Media", 1, 0, "Sound");

    qmlRegisterSingletonType<CDeviceInfo>("Cardirector.Device", 1, 0, "Device", cDeviceInfoProvider);

    if (p_ptr->isLoadedFromResource())
        addImportPath(QStringLiteral("qrc:/"));
}

CQmlEngine::~CQmlEngine()
{
    delete p_ptr;
}

