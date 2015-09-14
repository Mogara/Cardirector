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

#include <QObject>

MCD_BEGIN_NAMESPACE

class CAbstractPlayer;
class CServerAgent;
class CServerRobot;
class CRoom;
class CServerUser;

class CAbstractGameLogicPrivate;
class MCD_EXPORT CAbstractGameLogic : public QObject
{
    Q_OBJECT

public:
    explicit CAbstractGameLogic(CRoom *room);
    ~CAbstractGameLogic();

    void start();
    bool isRunning() const;

    CRoom *room() const;

    QMap<uint, CAbstractPlayer *> abstractPlayers() const;
    CAbstractPlayer *findAbstractPlayer(uint id) const;
    CAbstractPlayer *findAbstractPlayer(CServerAgent *agent) const;

    CServerAgent *findAgent(CAbstractPlayer *player) const;
    CServerUser *findUser(CAbstractPlayer *player) const;
    CServerRobot *findRobot(CAbstractPlayer *player) const;

signals:
    void started();
    void finished();

protected:
    virtual void run() = 0;

    //Parent must be the game logic
    virtual CAbstractPlayer *createPlayer(CServerUser *user) = 0;
    virtual CAbstractPlayer *createPlayer(CServerRobot *user) = 0;

private:
    C_DISABLE_COPY(CAbstractGameLogic)
    C_DECLARE_PRIVATE(CAbstractGameLogic)
    CAbstractGameLogicPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif // CABSTRACTGAMELOGIC_H

