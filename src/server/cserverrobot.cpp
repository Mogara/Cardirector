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

#include "croom.h"
#include "cserver.h"
#include "cserverrobot.h"
#include "cai.h"

#include <QSemaphore>

class CServerRobotPrivate
{
public:
    CAi *ai;
};

CServerRobot::CServerRobot(CRoom *room)
    : CServerAgent(room->server()), p_ptr(new CServerRobotPrivate)
{
    C_P(CServerRobot);
    p->ai = new CAi(this);

    static uint robotId = 0;
    robotId++;
    setId(robotId);
    setScreenName(room->newRobotName());

    room->addRobot(this);
}

CServerRobot::~CServerRobot()
{
    C_P(CServerRobot);
    delete p;
}

bool CServerRobot::initAi(const QString &aiStartScriptFile)
{
    C_P(CServerRobot);
    return p->ai->initAi(aiStartScriptFile);
}

void CServerRobot::request(int command, const QVariant &data, int)
{
    C_P(CServerRobot);
    p->ai->request(command, data);
}

void CServerRobot::reply(int command, const QVariant &data)
{
    C_P(CServerRobot);
    p->ai->reply(command, data);
}

void CServerRobot::notify(int command, const QVariant &data)
{
    C_P(CServerRobot);
    p->ai->notify(command, data);
}

void CServerRobot::executeRequest(int)
{
    C_P(CServerRobot);
    p->ai->request(requestCommand(), requestData());
}

void CServerRobot::cancelRequest()
{
}

void CServerRobot::setReplyReadySemaphore(QSemaphore *semaphore)
{
    C_P(CServerRobot);
    p->ai->setReplyReadySemaphore(semaphore);
}

QVariant CServerRobot::waitForReply()
{
    C_P(CServerRobot);
    return p->ai->waitForReply();
}

QVariant CServerRobot::waitForReply(int timeout)
{
    C_P(CServerRobot);
    return p->ai->waitForReply(timeout);
}

