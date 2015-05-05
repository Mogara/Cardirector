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

#include "cexceptionhandler.h"

#ifdef USE_BREAKPAD
#include <handler/exception_handler.h>
#endif

#include <QFile>

static CExceptionHandler::MinidumpCallback CDumpCallback = NULL;

#ifdef USE_BREAKPAD
#if defined(Q_OS_LINUX)
static bool GoogleBreakpadCallback(const google_breakpad::MinidumpDescriptor &md,void *, bool succeeded)
{
    if (succeeded && CDumpCallback)
        (*CDumpCallback)(md.path());
    return succeeded;
}

#elif defined(Q_OS_WIN)
static bool GoogleBreakpadCallback(const wchar_t *, const wchar_t *id, void *, EXCEPTION_POINTERS *, MDRawAssertionInfo *, bool succeeded)
{
    if (succeeded && CDumpCallback) {
        QString fileName = QString::fromWCharArray(id);
        fileName.append(".dmp");
        (*CDumpCallback)(fileName);
    }
    return succeeded;
}
#endif
#endif //USE_BREAKPAD

CExceptionHandler::CExceptionHandler(const QString &directory, MinidumpCallback callback)
{
    CDumpCallback = callback;

#ifdef USE_BREAKPAD
#if defined(Q_OS_LINUX)
    static google_breakpad::ExceptionHandler eh(google_breakpad::MinidumpDescriptor(directory.toStdString()), NULL, GoogleBreakpadCallback, NULL, true, -1);
#elif defined(Q_OS_WIN)
    static google_breakpad::ExceptionHandler eh(directory.toStdWString(), NULL, GoogleBreakpadCallback, NULL, google_breakpad::ExceptionHandler::HANDLER_ALL);
#endif
#else
    C_UNUSED(directory);
#endif  //USE_BREAKPAD
}
