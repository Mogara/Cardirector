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
class CClientUser;
class CAbstractPacketParser;

class MCD_EXPORT CClient : public QObject
{
    Q_OBJECT

public:
    CClient(QObject *parent = 0);
    ~CClient();

    void setPacketParser(CAbstractPacketParser *parser);
    CAbstractPacketParser *packetParser() const;

    void connectToHost(const QHostAddress &server, ushort port);

    void signup(const QString &username, const QString &password, const QString &screenName, const QString &avatar);
    void login(const QString &username, const QString &password);

    void createRoom();
    void enterRoom(uint id);
    void exitRoom();
    void speakToServer(const QString &message);

    void addRobot();
    void startGame();

    const CClientUser *findUser(uint id) const;
    QList<const CClientUser *> users() const;
    CClientUser *self() const;

    void fetchRoomList();

    void requestServer(int command, const QVariant &data = QVariant(), int timeout = -1);
    void replyToServer(int command, const QVariant &data = QVariant());
    void notifyServer(int command, const QVariant &data = QVariant());

    int requestTimeout() const;

    QVariant waitForReply();
    QVariant waitForReply(int timeout);

    static void AddInteraction(int command, void (*callback)(QObject *, const QVariant &));
    static void AddCallback(int command, void (*callback)(QObject *, const QVariant &));

signals:
    void connected();
    void loggedIn();
    void gameStarted();
    void userAdded(const CClientUser *user);
    void userRemoved(const CClientUser *user);
    void roomListUpdated(const QVariant &list);
    void roomEntered(const QVariant &config);
    void roomPropertyChanged(const QString &name, const QVariant &value);
    void systemMessage(const QString &message);

protected:
    CClientUser *findUser(uint id);
    CClientUser *addUser(const QVariant &data);

    static void SetUserListCommand(QObject *receiver, const QVariant &data);
    static void AddUserCommand(QObject *receiver, const QVariant &data);
    static void RemoveUserCommand(QObject *receiver, const QVariant &data);
    static void LoginCommand(QObject *receiver, const QVariant &data);
    static void SetRoomListCommand(QObject *receiver, const QVariant &data);
    static void SpeakCommand(QObject *receiver, const QVariant &data);
    static void EnterRoomCommand(QObject *receiver, const QVariant &data);
    static void UpdateRoomPropertyCommand(QObject *receiver, const QVariant &data);
    static void NetworkDelayCommand(QObject *receiver, const QVariant &data);
    static void StartGameCommand(QObject *receiver, const QVariant &);

private:
    C_DECLARE_INITIALIZER(CClient)

    C_DISABLE_COPY(CClient)
    C_DECLARE_PRIVATE(CClient)
    CClientPrivate *p_ptr;
};

MCD_END_NAMESPACE
