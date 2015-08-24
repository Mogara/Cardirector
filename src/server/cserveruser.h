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

#ifndef CSERVERUSER_H
#define CSERVERUSER_H

#include "cabstractserveruser.h"

#include <QObject>
#include <QHostAddress>

MCD_BEGIN_NAMESPACE

class CAbstractPacketParser;
class CTcpSocket;

class CServerUserPrivate;
class MCD_EXPORT CServerUser : public CAbstractServerUser
{
    Q_OBJECT

public:
    CServerUser(CTcpSocket *socket, CServer *server = 0);
    ~CServerUser();

    void setSocket(CTcpSocket *socket);

    void signup(const QString &username, const QString &password, const QString &screenName, const QString &avatar);
    void login(const QString &username, const QString &password);
    void logout();
    void kick();
    QHostAddress ip() const;

    void updateNetworkDelay();

    void request(int command, const QVariant &data = QVariant(), int timeout = -1);
    void reply(int command, const QVariant &data = QVariant());
    void notify(int command, const QVariant &data = QVariant());

    void prepareRequest(int command, const QVariant &data = QVariant());
    void executeRequest(int timeout = -1);
    void cancelRequest();

    QVariant waitForReply();
    QVariant waitForReply(int timeout);

    static void AddInteraction(int command, void (*callback)(QObject *, const QVariant &));
    static void AddCallback(int command, void (*callback)(QObject *, const QVariant &));

    bool controlledByClient() const {    return true;    }

signals:
    void disconnected();
    void replyReady();

protected:
    static void CheckVersionCommand(QObject *receiver, const QVariant &data);
    static void SignupCommand(QObject *receiver, const QVariant &data);
    static void LoginCommand(QObject *receiver, const QVariant &data);
    static void LogoutCommand(QObject *receiver, const QVariant &);
    static void SpeakCommand(QObject *receiver, const QVariant &data);
    static void CreateRoomCommand(QObject *receiver, const QVariant &data);
    static void EnterRoomCommand(QObject *receiver, const QVariant &data);
    static void NetworkDelayCommand(QObject *receiver, const QVariant &data);
    static void SetRoomListCommand(QObject *receiver, const QVariant &);
    static void GameStartCommand(QObject *receiver, const QVariant &);
    static void AddRobotCommand(QObject *receiver, const QVariant &);

    void handleUnknownPacket(const QByteArray &packet);

private:
    C_DECLARE_INITIALIZER(CServerUser)

    C_DISABLE_COPY(CServerUser)
    C_DECLARE_PRIVATE(CServerUser)
    CServerUserPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif // CSERVERUSER_H
