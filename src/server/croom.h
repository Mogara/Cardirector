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

#ifndef CROOM_H
#define CROOM_H

#include "cglobal.h"

#include <QObject>
#include <QVariant>

class CAbstractGameLogic;
class CAbstractUser;
class CRobot;
class CServer;
class CServerUser;

class CRoomPrivate;
class MCD_EXPORT CRoom : public QObject
{
    Q_OBJECT

public:
    CRoom(CServer *server);
    ~CRoom();

    uint id() const;
    QVariant config() const;

    CServer *server() const;

    void setOwner(CServerUser *owner);
    CServerUser *owner() const;

    QString name() const;
    void setName(const QString &name);

    int capacity() const;
    void setCapacity(int capacity);
    bool isFull() const;

    void setGameLogic(CAbstractGameLogic *gameLogic);
    CAbstractGameLogic *gameLogic() const;

    void addUser(CServerUser *user);
    void removeUser(CServerUser *user);

    CServerUser *findUser(uint id) const;
    QMap<uint, CServerUser *> users() const;

    void addRobot(CRobot *robot);
    void removeRobot(CRobot *robot);
    QString newRobotName() const;

    CRobot *findRobot(uint id) const;
    QMap<uint, CRobot *> robots();

    void startGame();

    void broadcastSystemMessage(const QString &message);

    void broadcastRequest(const QList<CServerUser *> &targets);
    void broadcastRequest(const QList<CServerUser *> &targets, int timeout);
    CServerUser *broadcastRacingRequest(const QList<CServerUser *> &targets, int timeout);
    void broadcastNotification(const QList<CServerUser *> &targets, int command, const QVariant &data = QVariant());
    void broadcastNotification(int command, const QVariant &data = QVariant(), CServerUser *except = NULL);

signals:
    void abandoned();
    void userAdded(CServerUser *user);
    void userRemoved(CServerUser *user);
    void robotAdded(CRobot *robot);
    void robotRemoved(CRobot *robot);

protected:
    //Slots for CServerUser. Do not call them directly.
    void onUserSpeaking(const QString &message);
    void onUserDisconnected();
    void onUserReplyReady();

    void onGameOver();

private:
    C_DISABLE_COPY(CRoom)
    C_DECLARE_PRIVATE(CRoom)
    CRoomPrivate *p_ptr;
};

#endif // CROOM_H
