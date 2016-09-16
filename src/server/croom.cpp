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
#include "croomsettings.h"
#include "cserver.h"
#include "cserveruser.h"

#include <QPointer>
#include <QSemaphore>
#include <QThread>
#include <QTime>

MCD_BEGIN_NAMESPACE

class CRoomPrivate
{
public:
    QPointer<CServer> server;
    uint id;
    CAbstractGameLogic *gameLogic;
    QMap<uint, CServerAgent *> agents;
    uint nextAgentId;
    CServerUser *owner;
    bool isAbandoned;
    CRoomSettings *settings;

    QSemaphore racingRequestSemaphore;
    QList<CServerAgent *> racingRequestCandidates;
    CServerAgent *racingRequestWinner;

    char robotNameCode;
    QThread *thread;

    CRoomPrivate()
        : gameLogic(Q_NULLPTR)
        , nextAgentId(0)
        , owner(Q_NULLPTR)
        , isAbandoned(false)
        , settings(new CRoomSettings)
        , robotNameCode('A')
    {
    }

    ~CRoomPrivate()
    {
        delete settings;
    }
};

CRoom::CRoom(CServer *server)
    : p_ptr(new CRoomPrivate)
{
    static uint roomId = 0;
    p_ptr->id = roomId;
    roomId++;
    p_ptr->server = server;

    p_ptr->thread = new QThread;
    connect(p_ptr->thread, &QThread::finished, p_ptr->thread, &QThread::deleteLater);
    connect(p_ptr->thread, &QThread::finished, this, &CRoom::deleteLater);
    p_ptr->thread->start();
}

CRoom::~CRoom()
{
    delete p_ptr;
}

uint CRoom::id() const
{
    return p_ptr->id;
}

QVariant CRoom::briefIntroduction() const
{
    QVariantMap info;
    info[QStringLiteral("id")] = (!p_ptr->server.isNull() && p_ptr->server->lobby() != this ? p_ptr->id : 0);
    info[QStringLiteral("name")] = name();
    info[QStringLiteral("userNum")] = p_ptr->agents.size();
    info[QStringLiteral("capacity")] = capacity();
    info[QStringLiteral("ownerId")] = ownerId();
    return info;
}

void CRoom::setSettings(CRoomSettings *settings)
{
    delete p_ptr->settings;
    p_ptr->settings = settings;
}

CRoomSettings *CRoom::settings() const
{
    return p_ptr->settings;
}

CServer *CRoom::server() const
{
    return p_ptr->server.data();
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
    return p_ptr->owner != Q_NULLPTR ? p_ptr->owner->id() : 0;
}

QString CRoom::name() const
{
    return p_ptr->settings->name;
}

void CRoom::setName(const QString &name)
{
    p_ptr->settings->name = name;
}

int CRoom::capacity() const
{
    return p_ptr->settings->capacity;
}

void CRoom::setCapacity(int capacity)
{
    p_ptr->settings->capacity = capacity;
}

bool CRoom::isFull() const
{
    return capacity() > 0 && p_ptr->agents.size() >= capacity();
}

bool CRoom::isAbandoned() const
{
    return p_ptr->isAbandoned;
}

void CRoom::setGameLogic(CAbstractGameLogic *gameLogic)
{
    if (p_ptr->gameLogic != Q_NULLPTR) {
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
    user->setId(nextAgentId());
    p_ptr->agents.insert(user->id(), user);
    user->setRoom(this);
    connect(user, &CServerUser::disconnected, this, &CRoom::onUserDisconnected);

    QVariantMap data;
    data[QStringLiteral("agentId")] = user->id();
    data[QStringLiteral("room")] = briefIntroduction();
    user->notify(S_COMMAND_ENTER_ROOM, data);
    unicastConfigTo(user);

    broadcastNotification(S_COMMAND_ADD_USER, user->briefIntroduction(), user);
    emit userAdded(user);
}

void CRoom::removeUser(CServerUser *user)
{
    if (p_ptr->agents.remove(user->id())) {
        user->disconnect(this);
        this->disconnect(user);

        if (user == p_ptr->owner) {
            p_ptr->owner = Q_NULLPTR;
            if (!p_ptr->agents.isEmpty()) {
                foreach (CServerAgent *agent, p_ptr->agents) {
                    if (agent->isHuman()) {
                        p_ptr->owner = agent->toUser();
                        break;
                    }
                }
                broadcastProperty("ownerId");
            }

            if (p_ptr->owner == Q_NULLPTR) {
                emit abandoned();
                p_ptr->isAbandoned = true;
                p_ptr->thread->quit();
                return;
            }
        }

        broadcastNotification(S_COMMAND_REMOVE_USER, user->id(), user);
        emit userRemoved(user);
    }
}

void CRoom::addRobot(CServerRobot *robot)
{
    robot->setId(nextAgentId());
    p_ptr->agents.insert(robot->id(), robot);
    robot->setRoom(this);

    broadcastNotification(S_COMMAND_ADD_ROBOT, robot->briefIntroduction());
    emit robotAdded(robot);
}

void CRoom::removeRobot(CServerRobot *robot)
{
    if (p_ptr->agents.remove(robot->id())) {
        this->disconnect(robot);
        robot->disconnect(this);

        broadcastNotification(S_COMMAND_REMOVE_ROBOT, robot->id());
        emit robotRemoved(robot);
    }
}

QString CRoom::newRobotName() const
{
    char code = p_ptr->robotNameCode;
    p_ptr->robotNameCode = p_ptr->robotNameCode + 1;

    if (p_ptr->robotNameCode == 'Z' + 1)
        p_ptr->robotNameCode = 'A';

    return tr("Robot %1").arg(code);
}

CServerUser *CRoom::findUser(uint id) const
{
    return qobject_cast<CServerUser *>(p_ptr->agents.value(id));
}

QMap<uint, CServerUser *> CRoom::users() const
{
    QMap<uint, CServerUser *> users;
    foreach (CServerAgent *agent, p_ptr->agents) {
        if (agent->isHuman())
            users.insert(agent->id(), agent->toUser());
    }
    return users;
}

CServerRobot *CRoom::findRobot(uint id) const
{
    return qobject_cast<CServerRobot *>(p_ptr->agents.value(id));
}

QMap<uint, CServerRobot *> CRoom::robots() const
{
    QMap<uint, CServerRobot *> robots;
    foreach (CServerAgent *agent, p_ptr->agents) {
        if (!agent->isHuman())
            robots.insert(agent->id(), agent->toRobot());
    }
    return robots;
}

QList<CServerAgent *> CRoom::agents() const
{
    return p_ptr->agents.values();
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
    QVariantMap data;
    data[QStringLiteral("message")] = message;
    broadcastNotification(S_COMMAND_SPEAK, data);
}

void CRoom::broadcastRequest()
{
    broadcastRequest(agents(), p_ptr->settings->timeout);
}

void CRoom::broadcastRequest(int timeout)
{
    broadcastRequest(agents(), timeout);
}

void CRoom::broadcastRequest(const QList<CServerAgent *> &targets)
{
    broadcastRequest(targets, p_ptr->settings->timeout);
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

    p_ptr->racingRequestSemaphore.tryAcquire(1, timeout * 1000);
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
    QMapIterator<uint, CServerAgent *> iter(p_ptr->agents);
    while (iter.hasNext()) {
        iter.next();
        uint id = iter.key();
        CServerRobot *robot = iter.value()->toRobot();
        if (robot) {
            removeRobot(robot);
            if (!p_ptr->server.isNull())
                p_ptr->server->killRobot(id);
            robot->deleteLater();
        }
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

void CRoom::unicastPropertyTo(const char *name, CServerAgent *agent) const
{
    QVariantMap data;
    data[QString::fromUtf8(name)] = property(name);
    agent->notify(S_COMMAND_CONFIGURE_ROOM, data);
}

void CRoom::broadcastProperty(const char *name) const
{
    QVariantMap data;
    data[QString::fromUtf8(name)] = property(name);
    broadcastNotification(S_COMMAND_CONFIGURE_ROOM, data);
}

void CRoom::unicastConfigTo(CServerAgent *agent) const
{
    agent->notify(S_COMMAND_CONFIGURE_ROOM, settings()->toVariant());
}

void CRoom::broadcastConfig() const
{
    broadcastNotification(S_COMMAND_CONFIGURE_ROOM, settings()->toVariant());
}

void CRoom::broadcastConfig(const QString &name) const
{
    QVariantMap data;
    data[name] = settings()->value(name);
    broadcastNotification(S_COMMAND_CONFIGURE_ROOM, data);
}

uint CRoom::nextAgentId()
{
    p_ptr->nextAgentId++;
    return p_ptr->nextAgentId;
}

void CRoom::onUserDisconnected()
{
    CServerUser *user = qobject_cast<CServerUser *>(sender());
    removeUser(user);
}

MCD_END_NAMESPACE
