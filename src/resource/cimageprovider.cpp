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

namespace
{
    const int StandardDPI = 96;
}

QPixmap CImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QVariant pathVar;
    QMetaObject::invokeMethod(this, "imagePath", Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QVariant, pathVar), Q_ARG(QVariant, id),
                              Q_ARG(QVariant, requestedSize));
    QUrl url = pathVar.toUrl();

    QPixmap pixmap;
    if (!url.isEmpty()) {
        if (url.scheme() == QStringLiteral("qrc")) {
            pixmap.load(QStringLiteral(":/") + url.path());
        } else if (url.scheme() == QStringLiteral("http")) {
            //@to-do: Load and store as a local file
        } else {
            pixmap.load(url.toString());
        }

        qreal localDPI = qApp->primaryScreen()->logicalDotsPerInch();
        if (StandardDPI != (int) localDPI) {
            const creal scaleFactor = StandardDPI / localDPI;
            if (!pixmap.isNull() && size != Q_NULLPTR)
                *size = pixmap.size() / scaleFactor;

            if (requestedSize.isValid()) {
                pixmap = pixmap.scaled(requestedSize, Qt::IgnoreAspectRatio,
                              Qt::SmoothTransformation);
            } else {
                pixmap = pixmap.scaled(*size, Qt::IgnoreAspectRatio,
                              Qt::SmoothTransformation);
            }
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
        if (window != Q_NULLPTR)
            window->addImageProvider(providerId, this);

        emit providerIdChanged();
    }
}

