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

#ifndef CIMAGEPROVIDER_H
#define CIMAGEPROVIDER_H

#include "cglobal.h"

#include <QObject>
#include <QQuickImageProvider>

MCD_BEGIN_NAMESPACE

class CImageProviderPrivate;

class MCD_EXPORT CImageProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT

    Q_PROPERTY(QString providerId READ providerId WRITE setProviderId NOTIFY providerIdChanged)

public:
    CImageProvider();
    ~CImageProvider();

    QPixmap requestPixmap(const QString &providerId, QSize *size, const QSize &requestedSize);

    QString providerId() const;
    void setProviderId(const QString &providerId);

signals:
    void providerIdChanged();

private:
    CImageProviderPrivate *p_ptr;
    C_DISABLE_COPY(CImageProvider)
    C_DECLARE_PRIVATE(CImageProvider)
};

MCD_END_NAMESPACE

#endif // CIMAGEPROVIDER_H
