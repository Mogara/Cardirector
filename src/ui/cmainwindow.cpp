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
#include "cqmlengine.h"

#include <QQuickItem>

class CMainWindowPrivate
{
public:
    CMainWindowPrivate()
        : settings(new QSettings)
    {

    }

    QSettings *settings;

    ~CMainWindowPrivate()
    {
        delete settings;
    }

    static CMainWindow *instance;
};

CMainWindow *CMainWindowPrivate::instance = NULL;

CMainWindow::CMainWindow(QWindow *parent)
    : QQuickView(new CQmlEngine, parent),
      p_ptr(new CMainWindowPrivate)
{
    setResizeMode(SizeRootObjectToView);

    if (p_ptr->instance == NULL)
        p_ptr->instance = this;

    restoreAsClosed();
}

CMainWindow *CMainWindow::mainInstance()
{
    return CMainWindowPrivate::instance;
}

void CMainWindow::registerMainInstance(CMainWindow *instance)
{
    Q_ASSERT(instance != NULL);

    CMainWindowPrivate::instance = instance;
}

CMainWindow::~CMainWindow()
{
    delete p_ptr;
    delete engine();
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
    }
    return QQuickView::event(e);
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

    QQmlContext *context = rootContext();
    context->setContextProperty("preferredSize", size);
    context->setContextProperty("Root", QVariant::fromValue(rootObject()));

    config->endGroup();
}

void CMainWindow::show()
{
    setVisible(true);
}
