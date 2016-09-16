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

#include "csoundeffect.h"
#include "coggfile.h"

#include <QAudioOutput>
#include <QBuffer>

MCD_BEGIN_NAMESPACE

class CSoundEffectPrivate
{
public:
    QBuffer *buffer;
    QAudioOutput *output;
    QString source;
    int loops;
    int loopsRemaining;
    qreal volume;

    CSoundEffectPrivate()
        : buffer(Q_NULLPTR)
        , output(Q_NULLPTR)
        , loops(1)
        , loopsRemaining(1)
        , volume(1.0)
    {
    }
};

CSoundEffect::CSoundEffect(QObject *parent)
    : QObject(parent)
    , p_ptr(new CSoundEffectPrivate)
{
}

CSoundEffect::CSoundEffect(const QString &source, QObject *parent)
    : QObject(parent)
    , p_ptr(new CSoundEffectPrivate)
{
    setSource(source);
}

CSoundEffect::~CSoundEffect()
{
    delete p_ptr;
}

QString CSoundEffect::source() const
{
    return p_ptr->source;
}

void CSoundEffect::setSource(const QString &source)
{
    p_ptr->source = source;
    emit sourceChanged();
}

void CSoundEffect::play()
{
    if (p_ptr->source.isEmpty())
        return;

    QIODevice *file = Q_NULLPTR;
    QAudioFormat format;

    //@to-do: if-else is not propriate to extend more formats
    if (p_ptr->source.endsWith(QStringLiteral(".ogg"))) {
        COggFile *ogg = new COggFile(p_ptr->source, this);
        if (ogg->open(QIODevice::ReadOnly)) {
            format = ogg->format();
            file = ogg;
        } else {
            delete ogg;
        }
    }

    if (file == Q_NULLPTR)
        return;

    if (p_ptr->output)
        p_ptr->output->deleteLater();
    if (p_ptr->buffer)
        p_ptr->buffer->deleteLater();

    p_ptr->buffer = new QBuffer(this);
    p_ptr->buffer->setData(file->readAll());
    delete file;
    p_ptr->buffer->open(QBuffer::ReadOnly);

    p_ptr->output = new QAudioOutput(format, this);
    p_ptr->output->setVolume(p_ptr->volume);
    connect(p_ptr->output, &QAudioOutput::stateChanged, this, &CSoundEffect::onOutputStateChanged);

    p_ptr->output->start(p_ptr->buffer);
    p_ptr->loopsRemaining = p_ptr->loops;
}

void CSoundEffect::stop()
{
    if (p_ptr->output)
        p_ptr->output->stop();
}

bool CSoundEffect::isPlaying() const
{
    return p_ptr->output && p_ptr->output->state() == QAudio::ActiveState;
}

int CSoundEffect::loops() const
{
    return p_ptr->loops;
}

int CSoundEffect::loopsRemaining() const
{
    return p_ptr->loopsRemaining;
}

void CSoundEffect::setLoops(int number)
{
    p_ptr->loopsRemaining = number;
    emit loopsRemainingChanged();

    p_ptr->loops = number;
    emit loopsChanged();
}

qreal CSoundEffect::volume() const
{
    return p_ptr->volume;
}

void CSoundEffect::setVolume(qreal volume)
{
    p_ptr->volume = volume;
    if (p_ptr->output)
        p_ptr->output->setVolume(volume);
    emit volumeChanged();
}

void CSoundEffect::onOutputStateChanged(QAudio::State state)
{
    emit isPlayingChanged();

    if (state == QAudio::ActiveState) {
        emit started();
    } else if (state == QAudio::IdleState || state == QAudio::StoppedState) {
        if ((p_ptr->loopsRemaining < p_ptr->loops || p_ptr->loops == Infinite) && p_ptr->output && p_ptr->buffer) {
            p_ptr->loopsRemaining++;
            emit loopsRemainingChanged();
            p_ptr->buffer->reset();
            p_ptr->output->start(p_ptr->buffer);
        } else {
            emit stopped();
        }
    }
}

MCD_END_NAMESPACE
