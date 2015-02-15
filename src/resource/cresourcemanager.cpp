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
#include "cresourcemanager.h"

#include <QCoreApplication>
#include <QCache>

QPointer<CResourceManager> CResourceManager::self;

class CResourceManagerPrivate
{
protected:
    explicit CResourceManagerPrivate(CResourceManager *const b)
        : b_ptr(b)
    {
        
    }

    QCache<QString, QPixmap> m_pixmapCache;

private:
    CResourceManager *const b_ptr;
    C_DECLARE_PUBLIC(CResourceManager);
    C_DISABLE_COPY(CResourceManagerPrivate);
};

// For atexit
namespace
{
    void cResourceManagerDestroyInstance()
    {
        if (CResourceManager::getInstance() != NULL)
            delete CResourceManager::getInstance();
    }
}

CResourceManager *CResourceManager::getInstance()
{
    if (self.isNull()) {
        self = new CResourceManager;

        if (qApp != NULL)
            connect(qApp, &QCoreApplication::aboutToQuit, self.data(), &CResourceManager::deleteLater);
        else
            atexit(&cResourceManagerDestroyInstance);
    }

    return self;
}

CResourceManager::CResourceManager()
    : p_ptr(new CResourceManagerPrivate(this))
{

}

QPixmap CResourceManager::getPixmap(const QString &path) {
    if (QFile::exists(path)) {
        C_P(CResourceManager);

        if (p->m_pixmapCache.contains(path))
            return *(p->m_pixmapCache.object(path));
        else {
            QPixmap *px = new QPixmap(path);
            if (!px->isNull()) {
                p->m_pixmapCache.insert(path, px);
                return *px;
            }
        }
    }

    Q_ASSERT(false);
    return QPixmap();
}
