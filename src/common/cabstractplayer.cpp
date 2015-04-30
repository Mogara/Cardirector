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
    QString screenName;
    QString avatar;
    CAbstractPlayer::State state;
};

CAbstractPlayer::CAbstractPlayer(QObject *parent)
    : QObject(parent)
    , p_ptr(new CAbstractPlayerPrivate)
{
    p_ptr->state = Online;
}

CAbstractPlayer::~CAbstractPlayer()
{

}

QString CAbstractPlayer::screenName() const
{
    return p_ptr->screenName;
}

void CAbstractPlayer::setScreenName(const QString &name)
{
    p_ptr->screenName = name;
}

QString CAbstractPlayer::avatar() const
{
    return p_ptr->avatar;
}

void CAbstractPlayer::setAvatar(const QString &avatar)
{
    p_ptr->avatar = avatar;
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
    case LoggedOut:
        return "logged_out";
    case Trust:
        return "trust";
    case Offline:
        return "offline";
    default:
        return "unknown";
    }
}

void CAbstractPlayer::setState(State state)
{
    p_ptr->state = state;
}

void CAbstractPlayer::setStateString(const QString &state)
{
    if (state == "online")
        p_ptr->state = Online;
    else if (state == "logged_out")
        p_ptr->state = LoggedOut;
    else if (state == "trust")
        p_ptr->state = Trust;
    else if (state == "offline")
        p_ptr->state = Offline;
    else
        qWarning("unknown state string");
}
