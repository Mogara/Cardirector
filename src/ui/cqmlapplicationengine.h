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

#ifndef CQMLAPPLICATIONENGINE_H
#define CQMLAPPLICATIONENGINE_H

#include "cglobal.h"

#include <QQmlApplicationEngine>

class MCD_EXPORT CQmlApplicationEngine : public QQmlApplicationEngine
{
    Q_OBJECT

public:
    CQmlApplicationEngine();
    ~CQmlApplicationEngine();

private:
    QString fileLocation() const;
    bool isLoadedFromResource() const;
};

#endif // CQMLAPPLICATIONENGINE_H
