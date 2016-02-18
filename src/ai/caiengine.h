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

#ifndef CAIENGINE_H
#define CAIENGINE_H

#include "cglobal.h"
#include <QJSEngine>

MCD_BEGIN_NAMESPACE
class CAiEnginePrivate;
class MCD_EXPORT CAiEngine : public QJSEngine
{
    Q_OBJECT

public:
    CAiEngine();
    ~CAiEngine();

public slots:
    void request(int command, QVariant data);
    void reply(int command, QVariant data);
    void notify(int command, QVariant data);

    void init(QString startScriptFile);

    bool avaliable() const;

signals:
    void replyReady(QVariant replyData);
    void notifyToRobot(int command, QVariant data);
    void initFinish(bool result);

private:
    C_DECLARE_PRIVATE(CAiEngine);
    CAiEnginePrivate *p_ptr;
};

class CAiEngineFunctions : public QObject
{
    Q_OBJECT

public:
    CAiEngineFunctions(CAiEngine *aiEngine);

public slots:
    Q_INVOKABLE void notifyToRobot(const QJSValue &command, const QJSValue &data);

private:
    CAiEngine *m_aiEngine;
};

MCD_END_NAMESPACE

#endif // CAIENGINE_H
