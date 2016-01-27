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

#include "csound.h"
#include "csoundeffect.h"

#include <QThread>
#include <QTimer>

MCD_BEGIN_NAMESPACE

class CSoundPrivate
{
public:
    CSoundEffect *effect;
    QThread *thread;

    CSoundPrivate()
        : effect(Q_NULLPTR)
        , thread(new QThread)
    {
        QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();
    }

    ~CSoundPrivate()
    {
        thread->quit();
    }
};

CSound::CSound(QObject *parent)
    : QObject(parent)
    , p_ptr(new CSoundPrivate)
{
}

CSound::CSound(const QString &fileName, QObject *parent)
    : QObject(parent)
    , p_ptr(new CSoundPrivate)
{
    setFileName(fileName);
}

CSound::~CSound()
{
    if (p_ptr->effect)
        p_ptr->effect->deleteLater();
    delete p_ptr;
}

QString CSound::fileName() const
{
    return p_ptr->effect ? p_ptr->effect->source() : QString();
}

void CSound::setFileName(const QString &fileName)
{
    int loops = 1;
    qreal volume = 1.0;

    if (p_ptr->effect) {
        if (p_ptr->effect->source() == fileName)
            return;
        loops = p_ptr->effect->loops();
        volume = p_ptr->effect->volume();

        this->disconnect(p_ptr->effect);
        p_ptr->effect->disconnect(this);
        p_ptr->effect->stop();
        p_ptr->effect->deleteLater();
    }

    p_ptr->effect = new CSoundEffect(fileName);
    p_ptr->effect->setVolume(volume);
    p_ptr->effect->setLoops(loops);
    connect(p_ptr->effect, &CSoundEffect::started, this, &CSound::started);
    connect(p_ptr->effect, &CSoundEffect::stopped, this, &CSound::stopped);
    connect(p_ptr->effect, &CSoundEffect::loopsChanged, this, &CSound::loopsChanged);
    connect(p_ptr->effect, &CSoundEffect::loopsRemainingChanged, this, &CSound::loopsRemainingChanged);
    connect(p_ptr->effect, &CSoundEffect::isPlayingChanged, this, &CSound::isPlayingChanged);
    connect(p_ptr->effect, &CSoundEffect::volumeChanged, this, &CSound::volumeChanged);

    p_ptr->effect->moveToThread(p_ptr->thread);

    emit fileNameChanged();
}

int CSound::loops() const
{
    return p_ptr->effect ? p_ptr->effect->loops() : 0;
}

int CSound::loopsRemaining() const
{
    return p_ptr->effect ? p_ptr->effect->loopsRemaining() : 0;
}

void CSound::setLoops(int number)
{
    if (p_ptr->effect)
        p_ptr->effect->setLoops(number);
}

bool CSound::isPlaying() const
{
    return p_ptr->effect ? p_ptr->effect->isPlaying() : false;
}

qreal CSound::volume() const
{
    return p_ptr->effect ? p_ptr->effect->volume() : 0.0;
}

void CSound::setVolume(qreal volume)
{
    if (p_ptr->effect)
        p_ptr->effect->setVolume(volume);
}

void CSound::play()
{
    if (p_ptr->effect)
        QTimer::singleShot(0, p_ptr->effect, &CSoundEffect::play);
}

void CSound::stop()
{
    if (p_ptr->effect)
        p_ptr->effect->stop();
}

void CSound::Play(const QString &fileName)
{
    CSound *sound = new CSound(fileName);
    connect(sound, &CSound::stopped, sound, &CSound::deleteLater);
    sound->play();
}

MCD_END_NAMESPACE
