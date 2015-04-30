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

#ifndef CABSTRACTPLAYER_H
#define CABSTRACTPLAYER_H

#include "cglobal.h"

#include <QObject>

MCD_BEGIN_NAMESPACE

class CAbstractPlayerPrivate;

class CAbstractPlayer : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString state READ stateString WRITE setStateString NOTIFY stateChanged)

    enum State{
        Online,
        Offline,
        LoggedOut,
        Trust
    };

    explicit CAbstractPlayer(QObject *parent = 0);
    ~CAbstractPlayer();

    QString screenName() const;
    void setScreenName(const QString &name);

    QString avatar() const;
    void setAvatar(const QString &avatar);

    State state() const;
    QString stateString() const;
    void setState(State state);
    void setStateString(const QString &state);

signals:
    void stateChanged();

private:
    C_DISABLE_COPY(CAbstractPlayer)
    C_DECLARE_PRIVATE(CAbstractPlayer)
    CAbstractPlayerPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif // CABSTRACTPLAYER_H
