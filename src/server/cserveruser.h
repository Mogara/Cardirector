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

#include "cserveragent.h"

#include <QObject>
#include <QHostAddress>

MCD_BEGIN_NAMESPACE

class CAbstractPacketParser;
class CTcpSocket;

class CServerUserPrivate;
class MCD_EXPORT CServerUser : public CServerAgent
{
    Q_OBJECT

public:
    CServerUser(CTcpSocket *socket, CServer *server = Q_NULLPTR);
    ~CServerUser();

    uint uid() const;

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

    void executeRequest(int timeout = -1);
    void cancelRequest();

    void setReplyReadySemaphore(QSemaphore *semaphore);
    QVariant waitForReply();
    QVariant waitForReply(int timeout);

    typedef void (*Callback)(CServerUser *user, const QVariant &);
    static void AddInteraction(int command, Callback callback);
    static void AddCallback(int command, Callback callback);

    bool isHuman() const;

signals:
    void disconnected();

protected:
    static void NetworkDelayCommand(CServerUser *user, const QVariant &data);

    void setUid(uint uid);
    void handleUnknownPacket(const QByteArray &packet);

private:
    C_DECLARE_INITIALIZER(CServerUser)

    C_DISABLE_COPY(CServerUser)
    C_DECLARE_PRIVATE(CServerUser)
    CServerUserPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif // CSERVERUSER_H
