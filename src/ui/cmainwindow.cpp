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

#include "cmainwindow.h"
#include "cqmlengine.h"
#include "cimageprovider.h"

#include <QDir>
#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickItem>
#include <QSettings>
#include <QScreen>

class CMainWindowPrivate
{
public:
    CMainWindowPrivate()
        : settings(new QSettings)
    {

    }

    QSettings *settings;
    QStringList imageProviders;

    ~CMainWindowPrivate()
    {
        delete settings;
    }

    static CMainWindow *instance;
};

CMainWindow *CMainWindowPrivate::instance = Q_NULLPTR;

CMainWindow::CMainWindow(QWindow *parent)
    : QQuickView(new CQmlEngine, parent),
      p_ptr(new CMainWindowPrivate)
{
    setResizeMode(SizeRootObjectToView);

    if (p_ptr->instance == Q_NULLPTR)
        p_ptr->instance = this;

    restoreAsClosed();

    QQmlContext *context = rootContext();
    context->setContextProperty(QStringLiteral("DPI"), qApp->primaryScreen()->logicalDotsPerInch());
}

CMainWindow *CMainWindow::mainInstance()
{
    return CMainWindowPrivate::instance;
}

void CMainWindow::registerMainInstance(CMainWindow *instance)
{
    Q_ASSERT(instance != Q_NULLPTR);

    CMainWindowPrivate::instance = instance;
}

CMainWindow::~CMainWindow()
{
    delete p_ptr;
    engine()->deleteLater();
}

bool CMainWindow::event(QEvent *e)
{
    if (e->type() == QEvent::Close) {
        QSettings *config = p_ptr->settings;
        config->beginGroup(QStringLiteral("MainWindow"));
        const Qt::WindowState state = windowState();
        if (!(state & (Qt::WindowMinimized | Qt::WindowMaximized | Qt::WindowFullScreen))) {
            config->setValue(QStringLiteral("size"), size());
            config->setValue(QStringLiteral("pos"), position());
        }
        config->setValue(QStringLiteral("state"), (int)state);
        config->endGroup();

        foreach(const QString &id, p_ptr->imageProviders)
            removeImageProvider(id);
    }
    return QQuickView::event(e);
}

void CMainWindow::restoreAsClosed()
{
    QSettings *config = p_ptr->settings;
    config->beginGroup(QStringLiteral("MainWindow"));

#ifndef MCD_FULLSCREEN_ONLY
    if (config->contains(QStringLiteral("pos")))
        setPosition(config->value(QStringLiteral("pos")).toPoint());
    if (config->contains(QStringLiteral("state")))
        setWindowState(Qt::WindowState(config->value(QStringLiteral("state")).toInt()));
    const QVariant size = config->value(QStringLiteral("size"), QSize(1024, 768));
        resize(size.toSize());
#endif

    config->endGroup();
}

void CMainWindow::show()
{
    setVisible(true);
}

void CMainWindow::addImageProvider(const QString &id, CImageProvider *provider)
{
    p_ptr->imageProviders << id;
    engine()->addImageProvider(id, provider);
}

void CMainWindow::removeImageProvider(const QString &id)
{
    p_ptr->imageProviders.removeOne(id);
    engine()->removeImageProvider(id);
}

void CMainWindow::setSource(const QUrl &source)
{
    QQuickView::setSource(source);
    QQmlContext *context = rootContext();
    context->setContextProperty(QStringLiteral("Root"), QVariant::fromValue(rootObject()));
    context->setContextProperty(QStringLiteral("LocalDirPath"), QDir::currentPath());
}
