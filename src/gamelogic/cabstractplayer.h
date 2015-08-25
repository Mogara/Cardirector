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

#ifndef CABSTRACTPLAYER_H
#define CABSTRACTPLAYER_H

#include "cglobal.h"

#include <QObject>

class CAbstractPlayerPrivate;
class MCD_EXPORT CAbstractPlayer : public QObject
{
    Q_OBJECT

    friend class CAbstractGameLogic;

public:
    CAbstractPlayer(QObject *parent = 0);
    ~CAbstractPlayer();

    uint id() const;

protected:
    void setId(uint id);

private:
    C_DISABLE_COPY(CAbstractPlayer)
    C_DECLARE_PRIVATE(CAbstractPlayer)
    CAbstractPlayerPrivate *p_ptr;
};

#endif // CABSTRACTPLAYER_H
