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
#include "cserverrobot.h"
#include "croom.h"
#include "cserver.h"
#include "cserveruser.h"

#include <QThread>
#include <QTime>
#include <QSemaphore>

class CRoomPrivate
{
public:
    CServer *server;
    uint id;
    QString name;
    CAbstractGameLogic *gameLogic;
    QMap<uint, CServerUser *> users;
    QMap<uint, CServerRobot *> robots;
    int capacity;
    CServerUser *owner;

    QSemaphore racingRequestSemaphore;
    QList<CServerAgent *> racingRequestCandidates;
    CServerAgent *racingRequestWinner;

    char robotNameCode;
    QThread *thread;
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

    p_ptr->thread = new QThread(this);
    p_ptr->thread->start();
}

CRoom::~CRoom()
{
    if (p_ptr->thread->isRunning()) {
        connect(p_ptr->thread, &QThread::finished, p_ptr->thread, &QThread::deleteLater);
        p_ptr->thread->setParent(NULL);
        p_ptr->thread->quit();
    }
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
    info["userNum"] = p_ptr->users.size() + p_ptr->robots.size();
    info["capacity"] = capacity();
    info["ownerId"] = ownerId();
    return info;
}

CServer *CRoom::server() const
{
    return p_ptr->server;
}

void CRoom::setOwner(CServerUser *owner)
{
    p_ptr->owner = owner;
}

CServerUser *CRoom::owner() const
{
    return p_ptr->owner;
}

uint CRoom::ownerId() const
{
    return p_ptr->owner != NULL ? p_ptr->owner->id() : 0;
}

QString CRoom::name() const
{
    return p_ptr->name;
}

void CRoom::setName(const QString &name)
{
    p_ptr->name = name;
}

int CRoom::capacity() const
{
    return p_ptr->capacity;
}

void CRoom::setCapacity(int capacity)
{
    p_ptr->capacity = capacity;
}

bool CRoom::isFull() const
{
    return p_ptr->capacity > 0 && p_ptr->users.size() + p_ptr->robots.size() >= p_ptr->capacity;
}

void CRoom::setGameLogic(CAbstractGameLogic *gameLogic)
{
    if (p_ptr->gameLogic != NULL) {
        if (p_ptr->gameLogic->isRunning())
            return;
        gameLogic->deleteLater();
    }

    p_ptr->gameLogic = gameLogic;
    connect(this, &CRoom::aboutToStart, gameLogic, &CAbstractGameLogic::start);
    connect(gameLogic, &CAbstractGameLogic::started, this, &CRoom::started);
    connect(gameLogic, &CAbstractGameLogic::finished, this, &CRoom::onGameOver);
    connect(gameLogic, &CAbstractGameLogic::finished, this, &CRoom::finished);
    gameLogic->moveToThread(p_ptr->thread);
}

CAbstractGameLogic *CRoom::gameLogic() const
{
    return p_ptr->gameLogic;
}

void CRoom::addUser(CServerUser *user)
{
    if (isFull())
        return;

    //Exit the previous room
    CRoom *prevRoom = user->room();
    if (prevRoom)
        prevRoom->removeUser(user);

    //Update online user list
    QVariantList userList;
    int count = 0;
    QList<CServerAgent *> agents = this->agents();
    foreach (CServerAgent *other, agents) {
        userList << other->briefIntroduction();
        count++;
        //@todo: save the number 50 into CServerSettings
        if (count >= 50)
            break;
    }
    user->notify(S_COMMAND_SET_USER_LIST, userList);

    //Add the user
    p_ptr->users.insert(user->id(), user);
    user->setRoom(this);
    connect(user, &CServerUser::speak, this, &CRoom::onUserSpeaking);
    connect(user, &CServerUser::disconnected, this, &CRoom::onUserDisconnected);

    user->notify(S_COMMAND_ENTER_ROOM, config());
    broadcastNotification(S_COMMAND_ADD_USER, user->briefIntroduction(), user);
    emit userAdded(user);
}

void CRoom::removeUser(CServerUser *user)
{
    if (p_ptr->users.remove(user->id())) {
        user->disconnect(this);
        this->disconnect(user);

        if (user == p_ptr->owner) {
            if (!p_ptr->users.isEmpty()) {
                p_ptr->owner = p_ptr->users.first();
                notifyProperty("ownerId");
            } else {
                emit abandoned();
                deleteLater();
                return;
            }
        }

        broadcastNotification(S_COMMAND_REMOVE_USER, user->id(), user);
        emit userRemoved(user);
    }
}

void CRoom::addRobot(CServerRobot *robot)
{
    p_ptr->robots.insert(robot->id(), robot);
    robot->setRoom(this);

    broadcastNotification(S_COMMAND_ADD_ROBOT, robot->briefIntroduction());
    emit robotAdded(robot);
}

void CRoom::removeRobot(CServerRobot *robot)
{
    if (p_ptr->robots.remove(robot->id())) {
        broadcastNotification(S_COMMAND_REMOVE_ROBOT, robot->id());
        emit robotRemoved(robot);
    }
}

QString CRoom::newRobotName() const
{
    char code = p_ptr->robotNameCode;
    p_ptr->robotNameCode = p_ptr->robotNameCode + 1;

    return tr("Robot %1").arg(code);
}

CServerUser *CRoom::findUser(uint id) const
{
    return p_ptr->users.value(id);
}

QMap<uint, CServerUser *> CRoom::users() const
{
    return p_ptr->users;
}

CServerRobot *CRoom::findRobot(uint id) const
{
    return p_ptr->robots.value(id);
}

QMap<uint, CServerRobot *> CRoom::robots() const
{
    return p_ptr->robots;
}

QList<CServerAgent *> CRoom::agents() const
{
    QList<CServerAgent *> agents;
    foreach (CServerUser *user, p_ptr->users)
        agents << user;
    foreach (CServerRobot *robot, p_ptr->robots)
        agents << robot;
    return agents;
}

void CRoom::startGame()
{
    if (p_ptr->gameLogic && !p_ptr->gameLogic->isRunning()) {
        broadcastNotification(S_COMMAND_START_GAME);
        emit aboutToStart();
    }
}

void CRoom::broadcastSystemMessage(const QString &message)
{
    QVariantList data;
    data << QVariant() << message;
    broadcastNotification(S_COMMAND_SPEAK, data);
}

void CRoom::broadcastRequest()
{
    //@to-do: Add request timeout into the configuration of the room
    int timeout = 15;//seconds
    broadcastRequest(agents(), timeout);
}

void CRoom::broadcastRequest(int timeout)
{
    broadcastRequest(agents(), timeout);
}

void CRoom::broadcastRequest(const QList<CServerAgent *> &targets)
{
    //@to-do: Add request timeout into the configuration of the room
    int timeout = 15;//seconds
    broadcastRequest(targets, timeout);
}

void CRoom::broadcastRequest(const QList<CServerAgent *> &targets, int timeout)
{
    QSemaphore semaphore;
    foreach (CServerAgent *agent, targets)
        agent->setReplyReadySemaphore(&semaphore);

    foreach (CServerAgent *agent, targets)
        agent->executeRequest(timeout);

    semaphore.tryAcquire(targets.length(), timeout * 1000);
    foreach (CServerAgent *agent, targets)
        agent->cancelRequest();
}

CServerAgent *CRoom::broadcastRacingRequest(const QList<CServerAgent *> &targets, int timeout)
{
    p_ptr->racingRequestCandidates = targets;

    foreach (CServerAgent *agent, targets)
        connect(agent, &CServerAgent::replyReady, this, &CRoom::onAgentReplyReady);

    foreach (CServerAgent *user, targets)
        user->executeRequest(timeout);

    p_ptr->racingRequestSemaphore.acquire();
    return p_ptr->racingRequestWinner;
}

void CRoom::onAgentReplyReady()
{
    p_ptr->racingRequestWinner = qobject_cast<CServerAgent *>(sender());

    foreach (CServerAgent *agent, p_ptr->racingRequestCandidates) {
        if (agent == p_ptr->racingRequestWinner)
            continue;

        agent->cancelRequest();
        disconnect(agent, &CServerAgent::replyReady, this, &CRoom::onAgentReplyReady);
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

void CRoom::broadcastNotification(const QList<CServerAgent *> &targets, int command, const QVariant &data) const
{
    foreach (CServerAgent *user, targets)
        user->notify(command, data);
}

void CRoom::broadcastNotification(int command, const QVariant &data, CServerAgent *except) const
{
    QList<CServerAgent *> agents = this->agents();
    foreach (CServerAgent *agent, agents) {
        if (agent != except)
            agent->notify(command, data);
    }
}

void CRoom::notifyProperty(const char *name) const
{
    QVariantList data;
    data << name;
    data << property(name);
    broadcastNotification(S_COMMAND_UPDATE_ROOM_PROPERTY, data);
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
    removeUser(user);
}
