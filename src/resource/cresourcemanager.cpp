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

QPointer<CResourceManager> CResourceManager::self;

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
            connect(qApp, &QCoreApplication::aboutToQuit, self, &CResourceManager::deleteLater);
        else
            atexit(&cResourceManagerDestroyInstance);
    }

    return self;
}

CResourceManager::CResourceManager()
{

}

