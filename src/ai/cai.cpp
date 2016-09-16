/********************************************************************
    Copyright (c) 2013-2016 - Mogara

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

#include "cai.h"
#include "caiengine.h"

#include <QDebug>
#include <QThread>
#include <QSemaphore>

class CAiPrivate
{
public:

    QThread *thread;
    CAiEngine *aiEngine;

    bool aiEngineAvaliable;

    bool isRequesting;
    QVariant replyData;
    QSemaphore replyReadySemaphore;
    QSemaphore *extraReplyReadySemaphore;
};


CAi::CAi(QObject *parent)
    : QObject(parent), p_ptr(new CAiPrivate)
{
    C_P(CAi);

    p->thread = new QThread(this);
    p->thread->start();
    p->aiEngine = Q_NULLPTR;
    p->aiEngineAvaliable = false;
    p->isRequesting = false;
    p->extraReplyReadySemaphore = Q_NULLPTR;
}

CAi::~CAi()
{
    C_P(CAi);

    // make sure the semaphores are released
    p->replyReadySemaphore.release();
    if (p->extraReplyReadySemaphore != Q_NULLPTR)
        p->extraReplyReadySemaphore->release();

    p->thread->exit();
    if (!p->thread->wait(2000))
        p->thread->terminate();

    if (!p->thread->wait(2000))
        qDebug() << QStringLiteral("CAi::~CAi: QThread didn't finish termination in 2 seconds, force deleting");

    if (p->aiEngine != Q_NULLPTR) {
        p->aiEngine->collectGarbage();
        delete p->aiEngine;
    }

    delete p;
}

void CAi::request(int command, const QVariant &data)
{
    C_P(CAi);
    if (p->replyReadySemaphore.available() > 0)
        p->replyReadySemaphore.acquire(p->replyReadySemaphore.available());

    p->replyData = QVariant();
    p->isRequesting = true;

    emit requestToAiEngine(command, data);
}

void CAi::reply(int command, const QVariant &data)
{
    emit replyToAiEngine(command, data);
}

void CAi::notify(int command, const QVariant &data)
{
    emit notifyToAiEngine(command, data);
}

void CAi::initAi(const QString &aiStartScriptFile)
{
    C_P(CAi);
    if (p->aiEngine != Q_NULLPTR)
        return;

    p->aiEngine = new CAiEngine;
    p->aiEngine->moveToThread(p->thread);

    connect(this, &CAi::initAiToAiEngine, p->aiEngine, &CAiEngine::init);
    connect(p->aiEngine, &CAiEngine::initFinish, this, &CAi::engineInitFinish);
    connect(p->aiEngine, &CAiEngine::replyReady, this, &CAi::engineReplyReady);
    connect(p->aiEngine, &CAiEngine::notifyToRobot, this, &CAi::engineNotifyToRobot);

    emit initAiToAiEngine(aiStartScriptFile);

    return;
}

void CAi::setReplyReadySemaphore(QSemaphore *semaphore)
{
    C_P(CAi);
    if (!p->isRequesting)
        p->extraReplyReadySemaphore = semaphore;
}

QVariant CAi::waitForReply(int timeout)
{
    C_P(CAi);
    p->replyReadySemaphore.tryAcquire(1, timeout);

    return p->replyData;
}

void CAi::engineReplyReady(QVariant replyData)
{
    C_P(CAi);
    if (!p->isRequesting)
        return;

    p->replyData = replyData;

    p->replyReadySemaphore.release();
    if (p->extraReplyReadySemaphore != Q_NULLPTR) {
        p->extraReplyReadySemaphore->release();
        p->extraReplyReadySemaphore = Q_NULLPTR;
    }
}

void CAi::engineNotifyToRobot(int command, QVariant data)
{
    emit notifyToRobot(static_cast<CCommandType>(command), data);
}

void CAi::engineInitFinish(bool result)
{
    C_P(CAi);
    if (result) {
        connect(this, &CAi::notifyToAiEngine, p->aiEngine, &CAiEngine::notify);
        connect(this, &CAi::replyToAiEngine, p->aiEngine, &CAiEngine::reply);
        connect(this, &CAi::requestToAiEngine, p->aiEngine, &CAiEngine::request);
        p->aiEngineAvaliable = true;
    } else {
        p->aiEngine->collectGarbage();
        delete p->aiEngine;
        p->aiEngine = Q_NULLPTR;
    }

    emit initFinish(result);
}

