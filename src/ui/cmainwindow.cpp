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
#include "cmainwindow.h"

#include <QtQml>

#include <QSettings>

//static const struct {
//    const char *type;
//        int major, minor;
//    } qmldir [] = {
//        { "MainWindow", 1, 0 }
//};

class CMainWindowPrivate
{
public:
    CMainWindowPrivate()
        : settings(new QSettings)
    {

    }

    QSettings *settings;

    CMainWindow *b_ptr;

    C_DECLARE_PUBLIC(CMainWindow)

    QString fileLocation() const
    {
        if (isLoadedFromResource())
            return "qrc:/Cardirector/Gui";
        return b_ptr->engine()->baseUrl().toString();
    }

    bool isLoadedFromResource() const
    {
        // If one file is missing, it will load all the files from the resource
//        QFile file(b_ptr->engine()->baseUrl().toLocalFile() + "/MainWindow.qml");
//        if (!file.exists())
//            return true;
        return false;
    }

    ~CMainWindowPrivate()
    {
        delete settings;
    }
};

CMainWindow::CMainWindow(QWindow *parent)
    : QQuickView(parent)
{
    init();
}

CMainWindow::CMainWindow(QQmlEngine *engine, QWindow *parent)
    : QQuickView(engine, parent)
{
    init();
}

CMainWindow::CMainWindow(const QUrl &source, QWindow *parent)
    : QQuickView(parent)
{
    init();
    setSource(source);
}

CMainWindow::~CMainWindow()
{
    delete p_ptr;
}

bool CMainWindow::event(QEvent *e)
{
    if (e->type() == QEvent::Close) {
        QSettings *config = p_ptr->settings;
        config->beginGroup("MainWindow");
        const Qt::WindowState state = windowState();
        if (!(state & (Qt::WindowMinimized | Qt::WindowMaximized | Qt::WindowFullScreen))) {
            config->setValue("size", size());
            config->setValue("pos", position());
        }
        config->setValue("state", (int)state);
        config->endGroup();
        return QQuickView::event(e);
    }
    return false;
}

void CMainWindow::restoreAsClosed()
{
    QSettings *config = p_ptr->settings;
    config->beginGroup("MainWindow");
    if (config->contains("pos"))
        setPosition(config->value("pos").toPoint());
    if (config->contains("state"))
        setWindowState(Qt::WindowState(config->value("state").toInt()));
    const QVariant size = config->value("size", QSize(1024, 768));
    rootContext()->setContextProperty("preferredSize", size);
    config->endGroup();
}

void CMainWindow::init()
{
    p_ptr = new CMainWindowPrivate;
    p_ptr->b_ptr = this;

    setResizeMode(SizeRootObjectToView);

//    const QString filesLocation = p_ptr->fileLocation();
//    for (int i = 0; i < int(sizeof(qmldir)/sizeof(qmldir[0])); i++)
//        qmlRegisterType(QUrl(filesLocation + "/" + qmldir[i].type + ".qml"), "Cardirector.Gui", qmldir[i].major, qmldir[i].minor, qmldir[i].type);

    if (p_ptr->isLoadedFromResource())
        engine()->addImportPath(QStringLiteral("qrc:/"));

    restoreAsClosed();
}
