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

#ifndef CSERVERAGENT_H
#define CSERVERAGENT_H

#include "cabstractuser.h"
#include <QVariant>

MCD_BEGIN_NAMESPACE

class CRoom;
class CServerRobot;
class CServer;
class CServerUser;
class QSemaphore;

class CServerAgentPrivate;
class MCD_EXPORT CServerAgent : public CAbstractUser
{
    Q_OBJECT

    friend class CRoom;

public:
    explicit CServerAgent(CServer *server = 0);
    ~CServerAgent();

    CServer *server() const;
    CRoom *room() const;
    void setRoom(CRoom *room);

    QVariant briefIntroduction() const;

    virtual void request(int command, const QVariant &data = QVariant(), int timeout = -1) = 0;
    virtual void reply(int command, const QVariant &data = QVariant()) = 0;
    virtual void notify(int command, const QVariant &data = QVariant()) = 0;

    void prepareRequest(int command, const QVariant &data = QVariant());
    virtual void setReplyReadySemaphore(QSemaphore *semaphore) = 0;
    virtual void executeRequest(int timeout = -1) = 0;
    virtual void cancelRequest() = 0;

    virtual QVariant waitForReply() = 0;
    virtual QVariant waitForReply(int timeout) = 0;
    virtual bool isHuman() const = 0;

    CServerUser *toUser();
    CServerRobot *toRobot();

    void broadcastProperty(const char *name);

signals:
    void replyReady();

protected:
    int requestCommand() const;
    QVariant requestData() const;

private:
    C_DISABLE_COPY(CServerAgent)
    C_DECLARE_PRIVATE(CServerAgent)
    CServerAgentPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif // CSERVERAGENT_H
