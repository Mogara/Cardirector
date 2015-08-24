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

#include "cabstractgamelogic.h"
#include "cprotocol.h"
#include "crobot.h"
#include "croom.h"
#include "cserver.h"
#include "cserveruser.h"

#include <QTime>
#include <QSemaphore>

class CRoomPrivate
{
public:
    CServer *server;
    uint id;
    QString name;
    CAbstractGameLogic *gameLogic;
    QMap<uint, CServerUser *> humanUsers;
    QMap<uint, CRobot *> robots;
    QMap<uint, CAbstractServerUser *> users;
    uint capacity;
    CServerUser *owner;

    QSemaphore racingRequestSemaphore;
    QList<CServerUser *> racingRequestCandidates;
    CServerUser *racingRequestWinner;

    char robotNameCode;

    void insertHumanUser(uint id, CServerUser *user)
    {
        humanUsers.insert(id, user);
        users.insert(id, user);
    }
    bool removeHumanUser(uint id)
    {
        humanUsers.remove(id);

        if (users.remove(id))
            return true;

        return false;
    }

    void insertRobot(uint id, CRobot *robot)
    {
        robots.insert(id, robot);
        users.insert(id, robot);
    }
    bool removeRobot(uint id)
    {
        robots.remove(id);

        if (users.remove(id))
            return true;

        return false;
    }
};

CRoom::CRoom(CServer *server)
    : QObject(server)
    , p_ptr(new CRoomPrivate)
{
    static uint roomId = 0;
    p_ptr->id = roomId;
    roomId++;
    p_ptr->server = server;
    p_ptr->gameLogic = NULL;
    p_ptr->owner = NULL;
    p_ptr->capacity = 0;
    p_ptr->robotNameCode = 'A';
}

CRoom::~CRoom()
{
    if (p_ptr->gameLogic)
        p_ptr->gameLogic->wait();
    delete p_ptr;
}

uint CRoom::id() const
{
    return p_ptr->id;
}

QVariant CRoom::config() const
{
    QVariantMap info;
    info["id"] = (p_ptr->server->lobby() != this ? p_ptr->id : 0);
    info["name"] = name();
    info["userNum"] = p_ptr->users.size();
    info["capacity"] = capacity();
    return info;
}

CServer *CRoom::server() const
{
    return p_ptr->server;
}

void CRoom::setOwner(CServerUser *owner)
{
    p_ptr->owner = owner;
    if (name().isEmpty())
        setName(tr("%1's Room").arg(owner->screenName()));
}

CServerUser *CRoom::owner() const
{
    return p_ptr->owner;
}

QString CRoom::name() const
{
    return p_ptr->name;
}

void CRoom::setName(const QString &name)
{
    p_ptr->name = name;
}

uint CRoom::capacity() const
{
    return p_ptr->capacity;
}

void CRoom::setCapacity(uint capacity)
{
    p_ptr->capacity = capacity;
}

bool CRoom::isFull() const
{
    return p_ptr->users.size() >= p_ptr->capacity;
}

void CRoom::setGameLogic(CAbstractGameLogic *gameLogic)
{
    if (p_ptr->gameLogic != NULL && p_ptr->gameLogic->isRunning())
        return;

    if (p_ptr->gameLogic != NULL)
        gameLogic->deleteLater();

    p_ptr->gameLogic = gameLogic;
    connect(gameLogic, &CAbstractGameLogic::gameOver, this, &CRoom::onGameOver);
}

CAbstractGameLogic *CRoom::gameLogic() const
{
    return p_ptr->gameLogic;
}

void CRoom::addHumanUser(CServerUser *user)
{
    if (isFull())
        return;

    //Exit the previous room
    CRoom *prevRoom = user->room();
    if (prevRoom)
        prevRoom->removeHumanUser(user);

    //Update online user list
    QVariantList userList;
    int count = 0;
    foreach (CAbstractServerUser *other, p_ptr->users) {
        userList << other->briefIntroduction();
        count++;
        //@todo: save the number 50 into CServerSettings
        if (count >= 50)
            break;
    }
    user->notify(S_COMMAND_SET_USER_LIST, userList);

    //Add the user
    p_ptr->insertHumanUser(user->id(), user);
    user->setRoom(this);
    connect(user, &CServerUser::speak, this, &CRoom::onUserSpeaking);
    connect(user, &CServerUser::disconnected, this, &CRoom::onUserDisconnected);

    user->notify(S_COMMAND_ENTER_ROOM, config());
    broadcastNotification(S_COMMAND_ADD_USER, user->briefIntroduction(), user);
    emit humanUserAdded(user);
}

void CRoom::removeHumanUser(CServerUser *user)
{
    if (p_ptr->removeHumanUser(user->id())) {
        user->disconnect(this);
        this->disconnect(user);

        if (user == p_ptr->owner) {
            if (!p_ptr->humanUsers.isEmpty()) {
                p_ptr->owner = p_ptr->humanUsers.first();
                //@todo: broadcast new owner
            } else {
                emit abandoned();
                deleteLater();
                return;
            }
        }

        broadcastNotification(S_COMMAND_REMOVE_USER, user->id(), user);
        emit humanUserRemoved(user);
    }
}

void CRoom::addRobot(CRobot *robot)
{
    p_ptr->insertRobot(robot->id(), robot);
    robot->setRoom(this);

    broadcastNotification(S_COMMAND_ADD_USER, robot->briefIntroduction());
    emit robotAdded(robot);
}

void CRoom::removeRobot(CRobot *robot)
{
    if (p_ptr->removeRobot(robot->id())) {
        broadcastNotification(S_COMMAND_REMOVE_USER, robot->id());
        emit robotRemoved(robot);
    }
}

QString CRoom::newRobotName() const
{
    char code = p_ptr->robotNameCode;
    p_ptr->robotNameCode = p_ptr->robotNameCode + 1;

    return tr("Robot %1").arg(code);
}

CAbstractServerUser *CRoom::findUser(uint id) const
{
    return p_ptr->users.value(id);
}

QMap<uint, CAbstractServerUser *> CRoom::users() const
{
    return p_ptr->users;
}

CServerUser *CRoom::findHumanUser(uint id) const
{
    return p_ptr->humanUsers.value(id);
}

QMap<uint, CServerUser *> CRoom::humanUsers() const
{
    return p_ptr->humanUsers;
}

CRobot *CRoom::findRobot(uint id) const
{
    return p_ptr->robots.value(id);
}

QMap<uint, CRobot *> CRoom::robots()
{
    return p_ptr->robots;
}

void CRoom::broadcastSystemMessage(const QString &message)
{
    QVariantList data;
    data << QVariant() << message;
    broadcastNotification(S_COMMAND_SPEAK, data);
}

void CRoom::broadcastRequest(const QList<CServerUser *> &targets)
{
    //@to-do: Add request timeout into the configuration of the room
    int timeout = 15;//seconds
    broadcastRequest(targets, timeout);
}

void CRoom::broadcastRequest(const QList<CServerUser *> &targets, int timeout)
{
    foreach (CServerUser *user, targets)
        user->executeRequest(timeout);
}

CServerUser *CRoom::broadcastRacingRequest(const QList<CServerUser *> &targets, int timeout)
{
    p_ptr->racingRequestCandidates = targets;

    foreach (CServerUser *user, targets)
        connect(user, &CServerUser::replyReady, this, &CRoom::onUserReplyReady);

    foreach (CServerUser *user, targets)
        user->executeRequest(timeout);

    p_ptr->racingRequestSemaphore.acquire();
    return p_ptr->racingRequestWinner;
}

void CRoom::onUserReplyReady()
{
    p_ptr->racingRequestWinner = qobject_cast<CServerUser *>(sender());

    foreach (CServerUser *user, p_ptr->racingRequestCandidates) {
        if (user == p_ptr->racingRequestWinner)
            continue;

        user->cancelRequest();
        disconnect(user, &CServerUser::replyReady, this, &CRoom::onUserReplyReady);
    }

    p_ptr->racingRequestSemaphore.release();
}

void CRoom::onGameOver()
{
    foreach (uint id, p_ptr->robots.keys()) {
        removeRobot(p_ptr->robots.value(id));
        p_ptr->server->killRobot(id);
    }
    p_ptr->robotNameCode = 'A';
}

void CRoom::broadcastNotification(const QList<CServerUser *> &targets, int command, const QVariant &data)
{
    foreach (CServerUser *user, targets)
        user->notify(command, data);
}

void CRoom::broadcastNotification(int command, const QVariant &data, CServerUser *except)
{
    foreach (CServerUser *user, p_ptr->humanUsers) {
        if (user != except)
            user->notify(command, data);
    }
}

void CRoom::onUserSpeaking(const QString &message)
{
    CServerUser *user = qobject_cast<CServerUser *>(sender());
    QVariantList arguments;
    arguments << user->id();
    arguments << message;
    broadcastNotification(S_COMMAND_SPEAK, arguments, user);
}

void CRoom::onUserDisconnected()
{
    CServerUser *user = qobject_cast<CServerUser *>(sender());
    removeHumanUser(user);
}
