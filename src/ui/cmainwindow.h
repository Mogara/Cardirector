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

#ifndef CWINDOW_H
#define CWINDOW_H

#include "cglobal.h"

#include <QQuickView>

MCD_BEGIN_NAMESPACE

class CMainWindowPrivate;
class CImageProvider;

class MCD_EXPORT CMainWindow : public QQuickView
{
public:
    explicit CMainWindow(QWindow *parent = 0);

    static CMainWindow *mainInstance();
    static void registerMainInstance(CMainWindow *instance);

    virtual ~CMainWindow();

    void show();

    void addImageProvider(const QString &id, CImageProvider *provider);
    void removeImageProvider(const QString &id);

public slots:
    void setSource(const QUrl &source);

protected:
    bool event(QEvent *e);
    void restoreAsClosed();

private:
    CMainWindowPrivate *p_ptr;
    C_DISABLE_COPY(CMainWindow)
    C_DECLARE_PRIVATE(CMainWindow)
};

#define MCD_INIT_GUI \
    Q_INIT_RESOURCE(gui); \
    Q_INIT_RESOURCE(font); \
    Q_INIT_RESOURCE(image)

MCD_END_NAMESPACE

#endif // CWINDOW_H
