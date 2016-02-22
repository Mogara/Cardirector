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

#ifndef CQMLENGINE_H
#define CQMLENGINE_H

#include "cglobal.h"

#include <QQmlEngine>

MCD_BEGIN_NAMESPACE

class CQmlEnginePrivate;

class MCD_EXPORT CQmlEngine : public QQmlEngine
{
public:
    CQmlEngine(QObject *p = Q_NULLPTR);
    ~CQmlEngine();

private:
    CQmlEnginePrivate *p_ptr;

    C_DISABLE_COPY(CQmlEngine)
    C_DECLARE_PRIVATE(CQmlEngine)
};

MCD_END_NAMESPACE

#endif // CQMLENGINE_H
