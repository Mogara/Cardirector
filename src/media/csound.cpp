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
#include "coggfile.h"

#include <QAudioOutput>
#include <QBuffer>

class CSoundPrivate
{
public:
    QBuffer *buffer;
    QAudioOutput *output;
    QString source;
    bool loop;
    qreal volume;

    CSoundPrivate()
        : buffer(NULL)
        , output(NULL)
        , loop(false)
        , volume(50)
    {
    }
};

CSound::CSound(QObject *parent)
    : QObject(parent)
    , p_ptr(new CSoundPrivate)
{
}

CSound::CSound(const QString &source, QObject *parent)
    : QObject(parent)
    , p_ptr(new CSoundPrivate)
{
    setSource(source);
}

CSound::~CSound()
{
    delete p_ptr;
}

QString CSound::source() const
{
    return p_ptr->source;
}

void CSound::setSource(const QString &source)
{
    p_ptr->source = source;

    QIODevice *file = NULL;
    QAudioFormat format;

    //@to-do: if-else is not propriate to extend more formats
    if (source.endsWith(".ogg")) {
        COggFile *ogg = new COggFile(source, this);
        if (ogg->open(QFile::ReadOnly)) {
            format = ogg->format();
            file = ogg;
        } else {
            delete ogg;
        }
    }

    if (file == NULL)
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
    connect(p_ptr->output, &QAudioOutput::stateChanged, this, &CSound::onOutputStateChanged);
}

void CSound::play()
{
    if (p_ptr->output && p_ptr->buffer)
        p_ptr->output->start(p_ptr->buffer);
}

void CSound::stop()
{
    if (p_ptr->output)
        p_ptr->output->stop();
}

bool CSound::isPlaying() const
{
    return p_ptr->output && p_ptr->output->state() == QAudio::ActiveState;
}

bool CSound::isLoop() const
{
    return p_ptr->loop;
}

void CSound::setLoop(bool loop)
{
    p_ptr->loop = loop;
}

qreal CSound::volume() const
{
    return p_ptr->volume;
}

void CSound::setVolume(qreal volume)
{
    p_ptr->volume = volume;
    if (p_ptr->output)
        p_ptr->output->setVolume(volume);
}

void CSound::onOutputStateChanged(QAudio::State state)
{
    if (state == QAudio::ActiveState) {
        emit started();
    } else if (state == QAudio::IdleState || state == QAudio::StoppedState) {
        emit stopped();
        if (p_ptr->loop && p_ptr->output && p_ptr->buffer) {
            p_ptr->buffer->reset();
            p_ptr->output->start(p_ptr->buffer);
        }
    }
}
