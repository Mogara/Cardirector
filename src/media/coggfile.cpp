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

#include "coggfile.h"

#if !defined(Q_OS_WIN) || !defined(Q_CC_MSVC)
#define OV_EXCLUDE_STATIC_CALLBACKS
#endif
#include <vorbis/vorbisfile.h>

class COggFilePrivate
{
public:
    OggVorbis_File file;
    QString filePath;
    int currentSection;
    COggFile::ReadError readError;

    COggFilePrivate()
        : currentSection(0)
        , readError(COggFile::NoReadError)
    {
    }
};

COggFile::COggFile(QObject *parent)
    : QIODevice(parent)
    , p_ptr(new COggFilePrivate)
{
}

COggFile::COggFile(const QString &filePath, QObject *parent)
    : QIODevice(parent)
    , p_ptr(new COggFilePrivate)
{
    p_ptr->filePath = filePath;
    ov_fopen(filePath.toLatin1().constData(), &p_ptr->file);
}

COggFile::~COggFile()
{
    close();
    delete p_ptr;
}

bool COggFile::open(QIODevice::OpenMode mode)
{
    if (0 == ov_fopen(p_ptr->filePath.toLatin1().constData(), &p_ptr->file)) {
        setOpenMode(mode);
        return true;
    } else {
        setOpenMode(NotOpen);
        return false;
    }
}

void COggFile::close()
{
    if (isOpen())
        ov_clear(&p_ptr->file);
}

QAudioFormat COggFile::format() const
{
    QAudioFormat format;
    format.setCodec(QStringLiteral("audio/pcm"));
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setChannelCount(p_ptr->file.vi->channels);
    format.setSampleRate(p_ptr->file.vi->rate);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    return format;
}

COggFile::ReadError COggFile::readError() const
{
    return p_ptr->readError;
}

bool COggFile::reset()
{
    return seek(0);
}

bool COggFile::seek(qint64 pos)
{
    return 0 == ov_pcm_seek(&p_ptr->file, pos);
}

qint64 COggFile::readData(char *data, qint64 maxlen)
{
    long result = ov_read(&p_ptr->file, data, maxlen, 0, 2, 1, &p_ptr->currentSection);
    if (result == OV_HOLE) {
        p_ptr->readError = InterruptedDataError;
        return 0;
    } else if (result == OV_EBADLINK) {
        p_ptr->readError = BadLinkError;
        return 0;
    } else if (result == OV_EINVAL) {
        p_ptr->readError = CorruptedHeaderError;
        return 0;
    }
    return result;
}

qint64 COggFile::writeData(const char *, qint64)
{
    //@to-do: ?
    return 0;
}
