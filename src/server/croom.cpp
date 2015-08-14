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

#include "cabstractgamelogic.h"
#include "cprotocol.h"
#include "croom.h"
#include "cserver.h"
#include "cserverplayer.h"

#include <QTime>
#include <QSemaphore>

class CRoomPrivate
{
public:
    CServer *server;
    uint id;
    QString name;
    CAbstractGameLogic *gameLogic;
    QMap<uint, CServerPlayer *> players;
    uint capacity;
    CServerPlayer *owner;

    QSemaphore racingRequestSemaphore;
    QList<CServerPlayer *> racingRequestCandidates;
    CServerPlayer *racingRequestWinner;
};

CRoom::CRoom(CServer *server)
    : QObject(server)
    , p_ptr(new CRoomPrivate)
{
    static uint roomId = 0;
    p_ptr->id = roomId;
    roomId++;
    p_ptr->server = server;
    p_ptr->gameLogic = NULL;
    p_ptr->owner = NULL;
    p_ptr->capacity = 0;
}

CRoom::~CRoom()
{
    if (p_ptr->gameLogic)
        p_ptr->gameLogic->wait();
    delete p_ptr;
}

uint CRoom::id() const
{
    return p_ptr->id;
}

QVariant CRoom::config() const
{
    QVariantMap info;
    info["id"] = (p_ptr->server->lobby() != this ? p_ptr->id : 0);
    info["name"] = name();
    info["playerNum"] = p_ptr->players.size();
    info["capacity"] = capacity();
    return info;
}

CServer *CRoom::server() const
{
    return p_ptr->server;
}

void CRoom::setOwner(CServerPlayer *owner)
{
    p_ptr->owner = owner;
    if (name().isEmpty())
        setName(tr("%1's Room").arg(owner->screenName()));
}

CServerPlayer *CRoom::owner() const
{
    return p_ptr->owner;
}

QString CRoom::name() const
{
    return p_ptr->name;
}

void CRoom::setName(const QString &name)
{
    p_ptr->name = name;
}

uint CRoom::capacity() const
{
    return p_ptr->capacity;
}

void CRoom::setCapacity(uint capacity)
{
    p_ptr->capacity = capacity;
}

void CRoom::setGameLogic(CAbstractGameLogic *gameLogic)
{
    p_ptr->gameLogic = gameLogic;
}

CAbstractGameLogic *CRoom::gameLogic() const
{
    return p_ptr->gameLogic;
}

void CRoom::addPlayer(CServerPlayer *player)
{
    //Exit the previous room
    CRoom *prevRoom = player->room();
    if (prevRoom)
        prevRoom->removePlayer(player);

    //Update online user list
    QVariantList playerList;
    int count = 0;
    foreach (CServerPlayer *other, p_ptr->players) {
        playerList << other->briefIntroduction();
        count++;
        //@todo: save the number 50 into CServerSettings
        if (count >= 50)
            break;
    }
    player->notify(S_COMMAND_SET_PLAYER_LIST, playerList);

    //Add the player
    p_ptr->players.insert(player->id(), player);
    player->setRoom(this);
    connect(player, &CServerPlayer::speak, this, &CRoom::onPlayerSpeaking);
    connect(player, &CServerPlayer::disconnected, this, &CRoom::onPlayerDisconnected);

    player->notify(S_COMMAND_ENTER_ROOM, config());
    broadcastNotification(S_COMMAND_ADD_PLAYER, player->briefIntroduction(), player);
    emit playerAdded(player);
}

void CRoom::removePlayer(CServerPlayer *player)
{
    if (p_ptr->players.remove(player->id())) {
        player->disconnect(this);
        this->disconnect(player);

        if (player == p_ptr->owner) {
            if (!p_ptr->players.isEmpty()) {
                p_ptr->owner = p_ptr->players.first();
                //@todo: broadcast new owner
            } else {
                emit abandoned();
                deleteLater();
                return;
            }
        }

        broadcastNotification(S_COMMAND_REMOVE_PLAYER, player->id(), player);
        emit playerRemoved(player);
    }
}

QMap<uint, CServerPlayer *> CRoom::players() const
{
    return p_ptr->players;
}

CServerPlayer *CRoom::findPlayer(int id) const
{
    return p_ptr->players.value(id);
}

void CRoom::broadcastSystemMessage(const QString &message)
{
    QVariantList data;
    data << QVariant() << message;
    broadcastNotification(S_COMMAND_SPEAK, data);
}

void CRoom::broadcastRequest(const QList<CServerPlayer *> &targets)
{
    //@to-do: Add request timeout into the configuration of the room
    int timeout = 15;//seconds
    broadcastRequest(targets, timeout);
}

void CRoom::broadcastRequest(const QList<CServerPlayer *> &targets, int timeout)
{
    foreach (CServerPlayer *player, targets)
        player->executeRequest(timeout);
}

CServerPlayer *CRoom::broadcastRacingRequest(const QList<CServerPlayer *> &targets, int timeout)
{
    p_ptr->racingRequestCandidates = targets;

    foreach (CServerPlayer *player, targets)
        connect(player, &CServerPlayer::replyReady, this, &CRoom::onPlayerReplyReady);

    foreach (CServerPlayer *player, targets)
        player->executeRequest(timeout);

    p_ptr->racingRequestSemaphore.acquire();
    return p_ptr->racingRequestWinner;
}

void CRoom::onPlayerReplyReady()
{
    p_ptr->racingRequestWinner = qobject_cast<CServerPlayer *>(sender());

    foreach (CServerPlayer *player, p_ptr->racingRequestCandidates) {
        if (player == p_ptr->racingRequestWinner)
            continue;

        player->cancelRequest();
        disconnect(player, &CServerPlayer::replyReady, this, &CRoom::onPlayerReplyReady);
    }

    p_ptr->racingRequestSemaphore.release();
}

void CRoom::broadcastNotification(const QList<CServerPlayer *> &targets, int command, const QVariant &data)
{
    foreach (CServerPlayer *player, targets)
        player->notify(command, data);
}

void CRoom::broadcastNotification(int command, const QVariant &data, CServerPlayer *except)
{
    foreach (CServerPlayer *player, p_ptr->players) {
        if (player != except)
            player->notify(command, data);
    }
}

void CRoom::onPlayerSpeaking(const QString &message)
{
    CServerPlayer *player = qobject_cast<CServerPlayer *>(sender());
    QVariantList arguments;
    arguments << player->id();
    arguments << message;
    broadcastNotification(S_COMMAND_SPEAK, arguments, player);
}

void CRoom::onPlayerDisconnected()
{
    CServerPlayer *player = qobject_cast<CServerPlayer *>(sender());
    removePlayer(player);
}
