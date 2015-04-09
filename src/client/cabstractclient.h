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

class CAbstractClientPrivate;

class MCD_EXPORT CAbstractClient : public QObject
{
    Q_OBJECT

public:
    CAbstractClient(QObject *parent = 0);
    ~CAbstractClient();

    void connectToHost(const QHostAddress &server, ushort port);

    void requestServer(int command, const QVariant &data = QVariant());
    void replyToServer(int command, const QVariant &data = QVariant());
    void notifyServer(int command, const QVariant &data = QVariant());

signals:
    void connected();

protected:
    void initCallbacks();
    static void SpeakCommand(QObject *client, const QVariant &data);

private:
    C_DISABLE_COPY(CAbstractClient)
    C_DECLARE_PRIVATE(CAbstractClient)
    CAbstractClientPrivate *p_ptr;
};

MCD_END_NAMESPACE
