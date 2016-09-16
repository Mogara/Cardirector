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

#include "cclient.h"
#include "cpacketrouter.h"
#include "ctcpsocket.h"
#include "cjsonpacketparser.h"
#include "cprotocol.h"
#include "cclientuser.h"

class CClientPrivate
{
public:
    CPacketRouter *router;
    QMap<uint, CClientUser *> users;
    CClientUser *self;
    CAbstractPacketParser *parser;
};

namespace
{
    QHash<int, CPacketRouter::Callback> interactions;
    QHash<int, CPacketRouter::Callback> callbacks;
}

CClient::CClient(QObject *parent)
    : QObject(parent)
    , p_ptr(new CClientPrivate)
{
    p_ptr->self = Q_NULLPTR;

    p_ptr->parser = new CJsonPacketParser;
    CTcpSocket *socket = new CTcpSocket;
    p_ptr->router = new CPacketRouter(this, socket, p_ptr->parser);
    p_ptr->router->setInteractions(&interactions);
    p_ptr->router->setCallbacks(&callbacks);
    connect(socket, &CTcpSocket::connected, this, &CClient::connected);
}

CClient::~CClient()
{
    delete p_ptr->router;
    delete p_ptr->parser;
    delete p_ptr;
}

void CClient::setPacketParser(CAbstractPacketParser *parser)
{
    delete p_ptr->parser;
    p_ptr->parser = parser;
}

CAbstractPacketParser *CClient::packetParser() const
{
    return p_ptr->parser;
}

void CClient::AddInteraction(int command, Callback callback)
{
    interactions.insert(command, reinterpret_cast<CPacketRouter::Callback>(callback));
}

void CClient::AddCallback(int command, Callback callback)
{
    callbacks.insert(command, reinterpret_cast<CPacketRouter::Callback>(callback));
}

void CClient::connectToHost(const QHostAddress &server, ushort port)
{
    CTcpSocket *socket = p_ptr->router->socket();
    socket->connectToHost(server, port);
}

void CClient::signup(const QString &username, const QString &password, const QString &screenName, const QString &avatar)
{
    QVariantList arguments;
    arguments << username << password << screenName << avatar;
    notifyServer(S_COMMAND_SIGNUP, arguments);
}

void CClient::login(const QString &username, const QString &password)
{
    QVariantList arguments;
    arguments << username << password;
    notifyServer(S_COMMAND_LOGIN, arguments);
}

void CClient::createRoom()
{
    p_ptr->router->notify(S_COMMAND_CREATE_ROOM);
}

void CClient::enterRoom(uint id)
{
    p_ptr->router->notify(S_COMMAND_ENTER_ROOM, id);
}

void CClient::exitRoom()
{
    p_ptr->router->notify(S_COMMAND_ENTER_ROOM);
}

void CClient::configureRoom(const QVariantMap &items)
{
    p_ptr->router->notify(S_COMMAND_CONFIGURE_ROOM, items);
}

void CClient::configureRoom(const QString &key, const QVariant &value)
{
    QVariantMap items;
    items[key] = value;
    configureRoom(items);
}

void CClient::speakToServer(const QString &message)
{
    return p_ptr->router->notify(S_COMMAND_SPEAK, message);
}

void CClient::toggleReady()
{
    if (p_ptr->self)
        p_ptr->router->notify(S_COMMAND_TOGGLE_READY, !p_ptr->self->isReady());
}

void CClient::addRobot()
{
    p_ptr->router->notify(S_COMMAND_ADD_ROBOT);
}

void CClient::startGame()
{
    p_ptr->router->notify(S_COMMAND_START_GAME);
}

const CClientUser *CClient::findUser(uint id) const
{
    return p_ptr->users.value(id);
}

QList<const CClientUser *> CClient::users() const
{
    QList<const CClientUser *> userList;
    foreach (const CClientUser *user, p_ptr->users)
        userList << user;
    return userList;
}

CClientUser *CClient::findUser(uint id)
{
    return p_ptr->users.value(id);
}

CClientUser *CClient::self() const
{
    return p_ptr->self;
}

void CClient::fetchRoomList()
{
    notifyServer(S_COMMAND_SET_ROOM_LIST);
}

CClientUser *CClient::addUser(const QVariant &data)
{
    QVariantList arguments = data.toList();
    if (arguments.length() < 3)
        return Q_NULLPTR;

    uint userId = arguments.at(0).toUInt();
    CClientUser *user = new CClientUser(userId, this);
    user->setScreenName(arguments.at(1).toString());
    user->setAvatar(arguments.at(2).toString());
    if (userId > 0)
        p_ptr->users.insert(userId, user);
    return user;
}

void CClient::requestServer(int command, const QVariant &data, int timeout)
{
    p_ptr->router->request(command, data, timeout);
}

void CClient::replyToServer(int command, const QVariant &data)
{
    p_ptr->router->reply(command, data);
}

void CClient::notifyServer(int command, const QVariant &data)
{
    p_ptr->router->notify(command, data);
}

int CClient::requestTimeout() const
{
    return p_ptr->router->requestTimeout();
}

QVariant CClient::waitForReply()
{
    return p_ptr->router->waitForReply();
}

QVariant CClient::waitForReply(int timeout)
{
    return p_ptr->router->waitForReply(timeout);
}

/* Callbacks */

void CClient::SetUserListCommand(CClient *client, const QVariant &data)
{
    QVariantList userList(data.toList());

    if (!client->p_ptr->users.isEmpty()) {
        foreach (CClientUser *user, client->p_ptr->users) {
            if (user != client->self())
                user->deleteLater();
        }
        client->p_ptr->users.clear();
        client->p_ptr->users.insert(client->self()->id(), client->self());
    }

    foreach (const QVariant &user, userList)
        client->addUser(user);
}

void CClient::AddUserCommand(CClient *client, const QVariant &data)
{
    CClientUser *user = client->addUser(data);
    emit client->userAdded(user);
}

void CClient::RemoveUserCommand(CClient *client, const QVariant &data)
{
    uint userId = data.toUInt();
    CClientUser *user = client->findUser(userId);
    if (user != Q_NULLPTR) {
        emit client->userRemoved(user);
        client->p_ptr->users.remove(userId);
        user->deleteLater();
    }
}

void CClient::LoginCommand(CClient *client, const QVariant &data)
{
    client->p_ptr->self = client->addUser(data);
    emit client->loggedIn();
}

void CClient::SetRoomListCommand(CClient *client, const QVariant &data)
{
    emit client->roomListUpdated(data);
}

void CClient::SpeakCommand(CClient *client, const QVariant &data)
{
    QVariantMap arguments = data.toMap();
    if (!arguments.contains(QStringLiteral("message")))
        return;
    QString message = arguments[QStringLiteral("message")].toString();

    if (arguments.size() == 1) {
        emit client->systemMessage(message);
    } else {
        if (arguments.contains(QStringLiteral("agentId"))) {
            CClientUser *user = client->findUser(arguments[QStringLiteral("agentId")].toUInt());
            if (user != Q_NULLPTR)
                emit user->speak(message);
        }
    }
}

void CClient::EnterRoomCommand(CClient *client, const QVariant &data)
{
    CClientUser *self = client->self();
    if (self == Q_NULLPTR) {
        qWarning("The user hasn't logged in.");
        return;
    }

    const QVariantMap arg = data.toMap();
    if (arg.contains(QStringLiteral("agentId")) && arg.contains(QStringLiteral("room"))) {
        uint agentId = arg[QStringLiteral("agentId")].toUInt();
        self->setId(agentId);
        client->p_ptr->users.insert(self->id(), self);
        emit client->roomEntered(arg[QStringLiteral("room")]);
    }
}

void CClient::ConfigureRoomCommand(CClient *client, const QVariant &data)
{
    QVariantMap properties = data.toMap();
    if (properties.isEmpty())
        return;

    for (QMapIterator<QString, QVariant> iter(properties); iter.hasNext(); ) {
        iter.next();
        // todo_Fs: the library should judge some of the room config like "ownerId", etc.
        emit client->roomConfigChanged(iter.key(), iter.value());
    }
}

void CClient::NetworkDelayCommand(CClient *client, const QVariant &data)
{
    client->notifyServer(S_COMMAND_NETWORK_DELAY, data);
}

void CClient::StartGameCommand(CClient *client, const QVariant &)
{
    emit client->gameStarted();
}

void CClient::AddRobotCommand(CClient *client, const QVariant &data)
{
    QVariantList arguments = data.toList();
    if (arguments.length() < 3)
        return;

    // to-do: implement the robot working in the server side
    uint agentId = arguments.at(0).toUInt();
    if (agentId > 0) {
        CClientUser *robot = new CClientUser(agentId, client);
        robot->setScreenName(arguments.at(1).toString());
        robot->setAvatar(arguments.at(2).toString());

        client->p_ptr->users.insert(agentId, robot);
        emit client->userAdded(robot);
    }
}

void CClient::UpdateUserPropertyCommand(CClient *client, const QVariant &data)
{
    QVariantList arguments = data.toList();
    if (arguments.length() < 2)
        return;

    QString name = arguments.at(0).toString();
    const QVariant value = arguments.at(1);

    CClientUser *user = Q_NULLPTR;
    if (arguments.length() > 2)
        user = client->findUser(arguments.at(0).toUInt());
    else
        user = client->self();

    if (user != Q_NULLPTR)
        user->setProperty(name.toLatin1(), value);
}

void CClient::Init()
{
    AddCallback(S_COMMAND_SPEAK, &SpeakCommand);
    AddCallback(S_COMMAND_SET_USER_LIST, &SetUserListCommand);
    AddCallback(S_COMMAND_ADD_USER, &AddUserCommand);
    AddCallback(S_COMMAND_REMOVE_USER, &RemoveUserCommand);
    AddCallback(S_COMMAND_LOGIN, &LoginCommand);
    AddCallback(S_COMMAND_SET_ROOM_LIST, &SetRoomListCommand);
    AddCallback(S_COMMAND_ENTER_ROOM, &EnterRoomCommand);
    AddCallback(S_COMMAND_CONFIGURE_ROOM, &ConfigureRoomCommand);
    AddCallback(S_COMMAND_NETWORK_DELAY, &NetworkDelayCommand);
    AddCallback(S_COMMAND_START_GAME, &StartGameCommand);
    AddCallback(S_COMMAND_ADD_ROBOT, &AddRobotCommand);
    AddCallback(S_COMMAND_UPDATE_USER_PROPERTY, &UpdateUserPropertyCommand);
}
C_INITIALIZE_CLASS(CClient)
