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

#ifndef CSERVERROBOT_H
#define CSERVERROBOT_H

#include "cserveragent.h"

MCD_BEGIN_NAMESPACE

class CServerRobotPrivate;
class MCD_EXPORT CServerRobot : public CServerAgent
{
    Q_OBJECT

public:
    explicit CServerRobot(CRoom *room);
    ~CServerRobot();

    void initAi(const QString &aiStartScriptFile);

    void request(int command, const QVariant &data = QVariant(), int timeout = -1);
    void reply(int command, const QVariant &data = QVariant());
    void notify(int command, const QVariant &data = QVariant());

    void executeRequest(int timeout = -1);
    void cancelRequest();

    void setReplyReadySemaphore(QSemaphore *semaphore);
    QVariant waitForReply();
    QVariant waitForReply(int timeout);

    bool controlledByClient() const { return false; }

signals:
    void aiInitFinish(bool result);

private:
    C_DISABLE_COPY(CServerRobot)
    C_DECLARE_PRIVATE(CServerRobot)
    CServerRobotPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif // CSERVERROBOT_H
