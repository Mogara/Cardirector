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

#include <QQmlContext>

class CMainWindowPrivate
{
public:
    CMainWindowPrivate(CClientSettings *clientSettings)
        : settings(clientSettings)
    {

    }

    CClientSettings *settings;
};


CMainWindow::CMainWindow(CClientSettings *stateRecorder, QWindow *parent)
    : QQuickView(parent), p_ptr(new CMainWindowPrivate(stateRecorder))
{
    restoreAsClosed();
}

CMainWindow::~CMainWindow()
{

}

bool CMainWindow::event(QEvent *e)
{
    if (p_ptr->settings != nullptr && e->type() == QEvent::Close) {
        if (!(windowState() & (Qt::WindowMaximized | Qt::WindowFullScreen))) {
            p_ptr->settings->setWindowSize(size());
            p_ptr->settings->setWindowPos(position());
        }
        p_ptr->settings->setWindowState(windowState());
    }
    return QQuickView::event(e);
}

void CMainWindow::restoreAsClosed()
{
    if (p_ptr != nullptr) {
        setPosition(p_ptr->settings->windowPos());
        setWindowState(p_ptr->settings->windowState());
        rootContext()->setContextProperty("windowSize", p_ptr->settings->windowSize());
    }
}
