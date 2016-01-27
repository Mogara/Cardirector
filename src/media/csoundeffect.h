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

#ifndef CSOUNDEFFECT_H
#define CSOUNDEFFECT_H

#include "cglobal.h"

#include <QQuickItem>
#include <QAudio>

MCD_BEGIN_NAMESPACE

class CSoundEffectPrivate;
class MCD_EXPORT CSoundEffect : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY(int loops READ loops WRITE setLoops NOTIFY loopsChanged)
    Q_PROPERTY(int loopsRemaining READ loopsRemaining NOTIFY loopsRemainingChanged)
    Q_PROPERTY(qreal volume READ volume WRITE setVolume NOTIFY volumeChanged)

public:
    enum Loop
    {
        Infinite = -1
    };

    CSoundEffect(QObject *parent = 0);
    CSoundEffect(const QString &source, QObject *parent = 0);
    ~CSoundEffect();

    QString source() const;
    void setSource(const QString &source);

    void play();
    void stop();

    bool isPlaying() const;

    int loops() const;
    int loopsRemaining() const;
    void setLoops(int number);

    qreal volume() const;
    void setVolume(qreal volume);

signals:
    void started();
    void stopped();

    void sourceChanged();
    void isPlayingChanged();
    void loopsChanged();
    void loopsRemainingChanged();
    void volumeChanged();

private:
    void onOutputStateChanged(QAudio::State state);

    C_DECLARE_PRIVATE(CSoundEffect)
    C_DISABLE_COPY(CSoundEffect)
    CSoundEffectPrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif
