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

#include "caiengine.h"

#include <QFile>
#include <QAtomicInt>

class CAiEnginePrivate
{
public:
    QAtomicInt available;

    CAiEnginePrivate()
        : available(0)
    {
    }
};

CAiEngine::CAiEngine()
    : p_ptr(new CAiEnginePrivate)
{
}

CAiEngine::~CAiEngine()
{
    C_P(CAiEngine);
    delete p;
}

void CAiEngine::request(int command, const QVariant &data)
{
    C_P(CAiEngine);
    if (p->available.load() == 0) {
        emit replyReady(QVariant());
        return;
    }
    QJSValue commandValue(command);
    QJSValue dataValue = toScriptValue(data);
    QJSValue requestFunction = globalObject().property("request");
    if (requestFunction.isCallable()) {
        QVariant r = requestFunction.call(QJSValueList() << commandValue << dataValue).toVariant();
        emit replyReady(r);
    }
}

void CAiEngine::reply(int command, const QVariant &data)
{
    C_P(CAiEngine);
    if (p->available.load() == 0)
        return;
    QJSValue commandValue(command);
    QJSValue dataValue = toScriptValue(data);
    QJSValue replyFunction = globalObject().property("reply");
    if (replyFunction.isCallable())
        replyFunction.call(QJSValueList() << commandValue << dataValue);
}

void CAiEngine::notify(int command, const QVariant &data)
{
    C_P(CAiEngine);
    if (p->available.load() == 0)
        return;
    QJSValue commandValue(command);
    QJSValue dataValue = toScriptValue(data);
    QJSValue notifyFunction = globalObject().property("notify");
    if (notifyFunction.isCallable())
        notifyFunction.call(QJSValueList() << commandValue << dataValue);
}

void CAiEngine::init(QString startScriptFile)
{
    C_P(CAiEngine);
    if (p->available.load() != 0)
        return;

    QFile file(startScriptFile);
    if (file.open(QFile::ReadOnly)) {
        QString s = file.readAll();
        file.close();

        evaluate(s);

        QJSValue initFunction = globalObject().property("init");
        if (initFunction.isCallable()) {
            bool succeed = initFunction.call().toBool();
            if (succeed)
                p->available.store(1);
        }
    }
}

bool CAiEngine::avaliable() const
{
    C_P(const CAiEngine);
    return p->available.load() != 0;
}
