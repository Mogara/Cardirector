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

#ifndef CCLIENTUSER_H
#define CCLIENTUSER_H

#include "cabstractuser.h"

MCD_BEGIN_NAMESPACE

class MCD_EXPORT CClientUser : public CAbstractUser
{
    Q_OBJECT

public:
    CClientUser(uint id, QObject *parent = 0);
    ~CClientUser();

signals:
    void speak(const QString &message);
};

MCD_END_NAMESPACE

#endif // CCLIENTUSER_H
