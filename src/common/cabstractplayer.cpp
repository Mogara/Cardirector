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

#include "cabstractplayer.h"

class CAbstractPlayerPrivate
{
public:
    uint id;
    QString screenName;
    QString avatar;
    CAbstractPlayer::State state;
    qint64 networkDelay;
};

CAbstractPlayer::CAbstractPlayer(QObject *parent)
    : QObject(parent)
    , p_ptr(new CAbstractPlayerPrivate)
{
    p_ptr->state = Invalid;
    p_ptr->id = 0;
}

CAbstractPlayer::~CAbstractPlayer()
{
    delete p_ptr;
}

uint CAbstractPlayer::id() const
{
    return p_ptr->id;
}

void CAbstractPlayer::setId(uint id)
{
    p_ptr->id = id;
}

QString CAbstractPlayer::screenName() const
{
    return p_ptr->screenName;
}

void CAbstractPlayer::setScreenName(const QString &name)
{
    p_ptr->screenName = name;
    emit screenNameChanged();
}

QString CAbstractPlayer::avatar() const
{
    return p_ptr->avatar;
}

void CAbstractPlayer::setAvatar(const QString &avatar)
{
    p_ptr->avatar = avatar;
    emit avatarChanged();
}

CAbstractPlayer::State CAbstractPlayer::state() const
{
    return p_ptr->state;
}

QString CAbstractPlayer::stateString() const
{
    switch (p_ptr->state) {
    case Online:
        return "online";
    case Trust:
        return "trust";
    case Offline:
        return "offline";
    default:
        return "invalid";
    }
}

void CAbstractPlayer::setState(State state)
{
    p_ptr->state = state;
    emit stateChanged();
}

void CAbstractPlayer::setStateString(const QString &state)
{
    if (state == "online")
        setState(Online);
    else if (state == "trust")
        setState(Trust);
    else if (state == "offline")
        setState(Offline);
    else
        setState(Invalid);
}

qint64 CAbstractPlayer::networkDelay() const
{
    return p_ptr->networkDelay;
}

void CAbstractPlayer::setNetworkDelay(qint64 delay)
{
    p_ptr->networkDelay = delay;
    emit networkDelayChanged();
}
