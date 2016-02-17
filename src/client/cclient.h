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
    void configureRoom(const QVariantMap &items);
    void configureRoom(const QString &key, const QVariant &value);
    void speakToServer(const QString &message);
    void toggleReady();

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

    typedef void (*Callback)(CClient *client, const QVariant &);
    static void AddInteraction(int command, Callback callback);
    static void AddCallback(int command, Callback callback);

signals:
    void connected();
    void loggedIn();
    void gameStarted();
    void userAdded(const CClientUser *user);
    void userRemoved(const CClientUser *user);
    void roomListUpdated(const QVariant &list);
    void roomEntered(const QVariant &config);
    void roomConfigChanged(const QString &key, const QVariant &value);
    void systemMessage(const QString &message);

protected:
    CClientUser *findUser(uint id);
    CClientUser *addUser(const QVariant &data);

    static void SetUserListCommand(CClient *client, const QVariant &data);
    static void AddUserCommand(CClient *client, const QVariant &data);
    static void RemoveUserCommand(CClient *client, const QVariant &data);
    static void LoginCommand(CClient *client, const QVariant &data);
    static void SetRoomListCommand(CClient *client, const QVariant &data);
    static void SpeakCommand(CClient *client, const QVariant &data);
    static void EnterRoomCommand(CClient *client, const QVariant &data);
    static void ConfigureRoomCommand(CClient *client, const QVariant &data);
    static void NetworkDelayCommand(CClient *client, const QVariant &data);
    static void StartGameCommand(CClient *client, const QVariant &);
    static void AddRobotCommand(CClient *client, const QVariant &data);
    static void UpdateUserPropertyCommand(CClient *client, const QVariant &data);

private:
    C_DECLARE_INITIALIZER(CClient)

    C_DISABLE_COPY(CClient)
    C_DECLARE_PRIVATE(CClient)
    CClientPrivate *p_ptr;
};

MCD_END_NAMESPACE
