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

#ifndef CABSTRACTUSER_H
#define CABSTRACTUSER_H

#include "cglobal.h"

#include <QObject>

MCD_BEGIN_NAMESPACE

class CAbstractUserPrivate;

class MCD_EXPORT CAbstractUser : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint id READ id)
    Q_PROPERTY(QString screenName READ screenName WRITE setScreenName NOTIFY screenNameChanged)
    Q_PROPERTY(QString state READ stateString WRITE setStateString NOTIFY stateChanged)
    Q_PROPERTY(QString avatar READ avatar WRITE setAvatar NOTIFY avatarChanged)
    Q_PROPERTY(qint64 networkDelay READ networkDelay WRITE setNetworkDelay NOTIFY networkDelayChanged)
    Q_PROPERTY(bool ready READ isReady WRITE setReady NOTIFY readyChanged)

public:
    enum State{
        Invalid,
        Online,
        Trust,
        Offline
    };

    explicit CAbstractUser(QObject *parent = Q_NULLPTR);
    ~CAbstractUser();

    uint id() const;

    QString screenName() const;
    void setScreenName(const QString &name);

    QString avatar() const;
    void setAvatar(const QString &avatar);

    State state() const;
    QString stateString() const;
    void setState(State state);
    void setStateString(const QString &state);

    qint64 networkDelay() const;
    void setNetworkDelay(qint64 delay);

    bool isReady() const;
    void setReady(bool ready);

protected:
    void setId(uint id);

signals:
    void screenNameChanged();
    void avatarChanged();
    void stateChanged();
    void networkDelayChanged();
    void readyChanged();

private:
    C_DISABLE_COPY(CAbstractUser)
    C_DECLARE_PRIVATE(CAbstractUser)
    CAbstractUserPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif // CABSTRACTUSER_H
