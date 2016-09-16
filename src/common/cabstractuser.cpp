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

#include "cabstractuser.h"

class CAbstractUserPrivate
{
public:
    uint id;
    QString screenName;
    QString avatar;
    CAbstractUser::State state;
    qint64 networkDelay;
    bool ready;

    CAbstractUserPrivate()
        : id(0)
        , state(CAbstractUser::Invalid)
        , networkDelay(10000)
        , ready(false)
    {
    }
};

CAbstractUser::CAbstractUser(QObject *parent)
    : QObject(parent)
    , p_ptr(new CAbstractUserPrivate)
{
}

CAbstractUser::~CAbstractUser()
{
    delete p_ptr;
}

uint CAbstractUser::id() const
{
    return p_ptr->id;
}

void CAbstractUser::setId(uint id)
{
    p_ptr->id = id;
}

QString CAbstractUser::screenName() const
{
    return p_ptr->screenName;
}

void CAbstractUser::setScreenName(const QString &name)
{
    p_ptr->screenName = name;
    emit screenNameChanged();
}

QString CAbstractUser::avatar() const
{
    return p_ptr->avatar;
}

void CAbstractUser::setAvatar(const QString &avatar)
{
    p_ptr->avatar = avatar;
    emit avatarChanged();
}

CAbstractUser::State CAbstractUser::state() const
{
    return p_ptr->state;
}

QString CAbstractUser::stateString() const
{
    switch (p_ptr->state) {
    case Online:
        return QStringLiteral("online");
    case Trust:
        return QStringLiteral("trust");
    case Offline:
        return QStringLiteral("offline");
    default:
        return QStringLiteral("invalid");
    }
}

void CAbstractUser::setState(State state)
{
    p_ptr->state = state;
    emit stateChanged();
}

void CAbstractUser::setStateString(const QString &state)
{
    if (state == QStringLiteral("online"))
        setState(Online);
    else if (state == QStringLiteral("trust"))
        setState(Trust);
    else if (state == QStringLiteral("offline"))
        setState(Offline);
    else
        setState(Invalid);
}

qint64 CAbstractUser::networkDelay() const
{
    return p_ptr->networkDelay;
}

void CAbstractUser::setNetworkDelay(qint64 delay)
{
    p_ptr->networkDelay = delay;
    emit networkDelayChanged();
}

bool CAbstractUser::isReady() const
{
    return p_ptr->ready;
}

void CAbstractUser::setReady(bool ready)
{
    p_ptr->ready = ready;
    emit readyChanged();
}
