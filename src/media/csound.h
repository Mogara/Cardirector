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

#include <QQuickItem>
#include <QAudio>

class CSoundPrivate;
class MCD_EXPORT CSound : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString source READ source WRITE setSource)
    Q_PROPERTY(bool isPlaying READ isPlaying)
    Q_PROPERTY(bool isLoop READ isLoop WRITE setLoop)
    Q_PROPERTY(qreal volume READ volume WRITE setVolume)

public:
    CSound(QQuickItem *parent = 0);
    ~CSound();

    QString source() const;
    void setSource(const QString &source);

    Q_INVOKABLE void play();
    Q_INVOKABLE void stop();

    bool isPlaying() const;

    bool isLoop() const;
    void setLoop(bool loop);

    qreal volume() const;
    void setVolume(qreal volume);

signals:
    void started();
    void stopped();

private:
    void onOutputStateChanged(QAudio::State state);

    C_DECLARE_PRIVATE(CSound)
    C_DISABLE_COPY(CSound)
    CSoundPrivate *p_ptr;
};
