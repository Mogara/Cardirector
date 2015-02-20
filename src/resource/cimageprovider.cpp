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

#include "cimageprovider.h"

#include "cmainwindow.h"

#include <QGuiApplication>
#include <QHash>
#include <QScreen>

class CImageProviderPrivate
{
public:

    CImageProviderPrivate()
    {

    }

    QString providerId;
};

CImageProvider::CImageProvider()
    : QQuickImageProvider(Pixmap, ForceAsynchronousImageLoading)
{
    p_ptr = new CImageProviderPrivate;
}

CImageProvider::~CImageProvider()
{
    delete p_ptr;
}

static const int StandardDPI = 96;

QPixmap CImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QVariant path;
    QMetaObject::invokeMethod(this, "imagePath", Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QVariant, path), Q_ARG(QVariant, id),
                              Q_ARG(QVariant, requestedSize));

    QPixmap pixmap;
    if (!path.toString().isEmpty()) {
        pixmap.load(path.toString());

        const creal scaleFactor = StandardDPI / qApp->primaryScreen()->logicalDotsPerInch();

        if (!pixmap.isNull() && size != NULL)
            *size = pixmap.size() / scaleFactor;

        if (requestedSize.isValid()) {
            pixmap = pixmap.scaled(requestedSize, Qt::IgnoreAspectRatio,
                          Qt::SmoothTransformation);
        } else {
            pixmap = pixmap.scaled(*size, Qt::IgnoreAspectRatio,
                          Qt::SmoothTransformation);
        }
    }

    return pixmap;
}

QString CImageProvider::providerId() const
{
    return p_ptr->providerId;
}

void CImageProvider::setProviderId(const QString &providerId)
{
    if (p_ptr->providerId != providerId) {
        p_ptr->providerId = providerId;

        CMainWindow *window = CMainWindow::mainInstance();
        if (window != NULL)
            window->addImageProvider(providerId, this);

        emit providerIdChanged();
    }
}

