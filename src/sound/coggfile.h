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

#ifndef COGGFILE_H
#define COGGFILE_H

#include "cglobal.h"

#include <QIODevice>
#include <QAudioFormat>

MCD_BEGIN_NAMESPACE

class COggFilePrivate;
class MCD_EXPORT COggFile : public QIODevice
{
public:
    enum ReadError
    {
        NoReadError,
        InterruptedDataError,
        BadLinkError,
        CorruptedHeaderError
    };

    COggFile(QObject *parent = 0);
    COggFile(const QString &filePath, QObject *parent = 0);
    ~COggFile();

    bool open(OpenMode mode);
    void close();

    QAudioFormat format() const;
    ReadError readError() const;

    bool reset();
    bool seek(qint64 pos);

protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private:
    C_DECLARE_PRIVATE(COggFile)
    C_DISABLE_COPY(COggFile)
    COggFilePrivate *p_ptr;
};

MCD_END_NAMESPACE

#endif // COGGFILE_H
