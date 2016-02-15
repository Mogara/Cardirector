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
class CServer;
class CServerAgent;
class CServerRobot;
class CServerUser;
class CRoomSettings;

class CRoomPrivate;
class MCD_EXPORT CRoom : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(uint ownerId READ ownerId)
    Q_PROPERTY(int capacity READ capacity WRITE setCapacity)
    Q_PROPERTY(bool isFull READ isFull)

public:
    CRoom(CServer *server);
    ~CRoom();

    uint id() const;
    QVariant briefIntroduction() const;

    void setSettings(CRoomSettings *settings);
    CRoomSettings *settings() const;

    template<typename T>
    const T *settings() const { return dynamic_cast<T *>(settings()); }

    CServer *server() const;

    void setOwner(CServerUser *owner);
    CServerUser *owner() const;
    uint ownerId() const;

    QString name() const;
    void setName(const QString &name);

    int capacity() const;
    void setCapacity(int capacity);
    bool isFull() const;

    bool isAbandoned() const;

    void setGameLogic(CAbstractGameLogic *gameLogic);
    CAbstractGameLogic *gameLogic() const;

    void addUser(CServerUser *user);
    void removeUser(CServerUser *user);

    CServerUser *findUser(uint id) const;
    QMap<uint, CServerUser *> users() const;

    void addRobot(CServerRobot *robot);
    void removeRobot(CServerRobot *robot);
    QString newRobotName() const;

    CServerRobot *findRobot(uint id) const;
    QMap<uint, CServerRobot *> robots() const;

    QList<CServerAgent *> agents() const;

    void startGame();

    void broadcastSystemMessage(const QString &message);

    void broadcastRequest();
    void broadcastRequest(int timeout);
    void broadcastRequest(const QList<CServerAgent *> &targets);
    void broadcastRequest(const QList<CServerAgent *> &targets, int timeout);
    CServerAgent *broadcastRacingRequest(const QList<CServerAgent *> &targets, int timeout);
    void broadcastNotification(const QList<CServerAgent *> &targets, int command, const QVariant &data = QVariant()) const;
    void broadcastNotification(int command, const QVariant &data = QVariant(), CServerAgent *except = NULL) const;

    void unicastPropertyTo(const char *name, CServerAgent *agent) const;
    void broadcastProperty(const char *name) const;
    void unicastConfigTo(CServerAgent *agent) const;
    void broadcastConfig() const;
    void broadcastConfig(const QString &name) const;

signals:
    void abandoned();

    void aboutToStart();
    void started();
    void finished();

    void userAdded(CServerUser *user);
    void userRemoved(CServerUser *user);
    void robotAdded(CServerRobot *robot);
    void robotRemoved(CServerRobot *robot);

protected:

    //Slots for CServerUser. Do not call them directly.
    void onUserSpeaking(const QString &message);
    void onUserDisconnected();
    void onAgentReplyReady();

    void onGameOver();

private:
    C_DISABLE_COPY(CRoom)
    C_DECLARE_PRIVATE(CRoom)
    CRoomPrivate *p_ptr;
};

#endif // CROOM_H
