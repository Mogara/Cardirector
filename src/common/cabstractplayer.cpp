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
};

CAbstractPlayer::CAbstractPlayer(QObject *parent)
    : QObject(parent)
    , p_ptr(new CAbstractPlayerPrivate)
{
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
