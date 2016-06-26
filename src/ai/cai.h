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

#ifndef CAI_H
#define CAI_H

#include "cglobal.h"
#include "cprotocol.h"

#include <QVariant>

class QSemaphore;

MCD_BEGIN_NAMESPACE

class CAiPrivate;
class MCD_EXPORT CAi : public QObject
{
    Q_OBJECT

public:
    explicit CAi(QObject *parent = Q_NULLPTR);
    ~CAi();

    void request(int command, const QVariant &data);
    void reply(int command, const QVariant &data);
    void notify(int command, const QVariant &data);

    void initAi(const QString &aiStartScriptFile);

    void setReplyReadySemaphore(QSemaphore *semaphore);

    QVariant waitForReply(int timeout = -1);

signals:
    // THESE 4 SIGNALS AND 2 SLOTS ARE used internally to notify the AI engine. DO NOT USED IN YOUR OWN PROGRAM!!!
    // todo_Fs: should make a seprate header file to maintain these signals and slots used internally. like "cai_cardirectorprivate.h"
    void requestToAiEngine(int command, QVariant data);
    void replyToAiEngine(int command, QVariant data);
    void notifyToAiEngine(int command, QVariant data);
    void initAiToAiEngine(QString aiStartScriptFile);

public slots:
    void engineInitFinish(bool result);
    void engineReplyReady(QVariant replyData);
    void engineNotifyToRobot(int command, QVariant data);

signals:
    // used in library to notify the AI system initialization is finished
    void initFinish(bool result);
    // used in library to transit the AI notification
    void notifyToRobot(CCommandType command, const QVariant &data);

private:
    C_DECLARE_PRIVATE(CAi);
    CAiPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif
