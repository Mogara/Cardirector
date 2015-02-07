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

#ifndef CRESOURCEMANAGER_H
#define CRESOURCEMANAGER_H

#include "cglobal.h"

#include <QPixmap>
#include <QPointer>

MCD_BEGIN_NAMESPACE

class CResourceManagerPrivate;

class MCD_EXPORT CResourceManager : public QObject
{
    Q_OBJECT

public:
    static CResourceManager *getInstance();

    QPixmap getPixmap(const QString &fileName); // @@TODO: No implemention now
    // CAudio getAudio(const QString &fileName); // @@TODO: implement CAudio


private:
    static QPointer<CResourceManager> self;

    CResourceManager();


    CResourceManagerPrivate *p_ptr;

    C_DISABLE_COPY(CResourceManager)
    C_DECLARE_PRIVATE(CResourceManager)
};

MCD_END_NAMESPACE

#endif // CRESOURCEMANAGER_H
