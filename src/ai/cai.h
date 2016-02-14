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

    bool initAi(const QString &aiStartScriptFile);

    void setReplyReadySemaphore(QSemaphore *semaphore);

    QVariant waitForReply(int timeout = -1);

signals:
    // used to notify the AI engine
    void requestToAiEngine(int command, const QVariant &data);
    void replyToAiEngine(int command, const QVariant &data);
    void notifyToAiEngine(int command, const QVariant &data);
    void initAiToAiEngine(QString aiStartScriptFile);

public slots:
    void engineReplyReady(QVariant replyData);

private:
    C_DECLARE_PRIVATE(CAi);
    CAiPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif
