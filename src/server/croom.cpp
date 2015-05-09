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

#include "cprotocol.h"
#include "croom.h"
#include "cserver.h"
#include "cserverplayer.h"

#include <QMultiMap>

class CRoomPrivate
{
public:
    CServer *server;
    CAbstractGameLogic *gameLogic;
    QMap<uint, CServerPlayer *> players;
};

CRoom::CRoom(CServer *server)
    : QObject(server)
    , p_ptr(new CRoomPrivate)
{
    p_ptr->server = server;
    p_ptr->gameLogic = NULL;
}

CRoom::~CRoom()
{
    delete p_ptr;
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

    broadcastNotification(S_COMMAND_ADD_PLAYER, player->briefIntroduction(), player);
}

void CRoom::removePlayer(CServerPlayer *player)
{
    if (p_ptr->players.remove(player->id())) {
        player->disconnect(this);
        this->disconnect(player);
        broadcastNotification(S_COMMAND_REMOVE_PLAYER, player->id(), player);
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
