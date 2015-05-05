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

#include "cglobal.h"

#include <QObject>
#include <QHostAddress>

MCD_BEGIN_NAMESPACE

class CClientPrivate;
class CClientPlayer;

class MCD_EXPORT CClient : public QObject
{
    Q_OBJECT

public:
    CClient(QObject *parent = 0);
    ~CClient();

    void connectToHost(const QHostAddress &server, ushort port);

    void signup(const QString &username, const QString &password, const QString &screenName, const QString &avatar);
    void login(const QString &username, const QString &password);

    void requestServer(int command, const QVariant &data = QVariant());
    void replyToServer(int command, const QVariant &data = QVariant());
    void notifyServer(int command, const QVariant &data = QVariant());

    const CClientPlayer *findPlayer(uint id) const;
    QList<const CClientPlayer *> players() const;
    CClientPlayer *self() const;


signals:
    void connected();
    void loggedIn();
    void playerAdded(const CClientPlayer *player);
    void playerRemoved(const CClientPlayer *player);

protected:
    CClientPlayer *findPlayer(uint id);
    void addCallback(int command, void (*callback)(QObject *, const QVariant &));

    CClientPlayer *addPlayer(const QVariant &data);

    void initCallbacks();
    static void SetPlayerListCommand(QObject *receiver, const QVariant &data);
    static void AddPlayerCommand(QObject *receiver, const QVariant &data);
    static void RemovePlayerCommand(QObject *receiver, const QVariant &data);
    static void LoginCommand(QObject *receiver, const QVariant &data);
    static void SpeakCommand(QObject *receiver, const QVariant &data);

private:
    C_DISABLE_COPY(CClient)
    C_DECLARE_PRIVATE(CClient)
    CClientPrivate *p_ptr;
};

MCD_END_NAMESPACE
