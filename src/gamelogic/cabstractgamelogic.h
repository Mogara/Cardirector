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

#ifndef CABSTRACTGAMELOGIC_H
#define CABSTRACTGAMELOGIC_H

#include "cglobal.h"

#include <QThread>

MCD_BEGIN_NAMESPACE

class CAbstractPlayer;
class CAbstractServerUser;
class CRobot;
class CRoom;
class CServerUser;

class CAbstractGameLogicPrivate;
class MCD_EXPORT CAbstractGameLogic : public QThread
{
    Q_OBJECT

    friend class CAbstractPlayer;

public:
    explicit CAbstractGameLogic(CRoom *parent = 0);
    ~CAbstractGameLogic();

    void start(Priority priority = InheritPriority);

    CRoom *room() const;
    CAbstractServerUser *findAbstractUser(CAbstractPlayer *player);

signals:
    void gameOver();

protected:
    uint createPlayerId();

    //Parent must be the game logic
    virtual CAbstractPlayer *createPlayer(CServerUser *user) = 0;
    virtual CAbstractPlayer *createPlayer(CRobot *user) = 0;

private:
    C_DISABLE_COPY(CAbstractGameLogic)
    C_DECLARE_PRIVATE(CAbstractGameLogic)
    CAbstractGameLogicPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif // CABSTRACTGAMELOGIC_H

