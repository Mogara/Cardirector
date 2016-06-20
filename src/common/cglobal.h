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

#ifndef CGLOBAL_H
#define CGLOBAL_H

#include <QtCore/qglobal.h>
#include <QCoreApplication>

#if 0
class MCD_EXPORT CardirectorGlobal
#endif // for header file auto-generation

#ifndef MCD_STATIC
#ifdef MCD_BUILD
#define MCD_EXPORT Q_DECL_EXPORT
#else
#define MCD_EXPORT Q_DECL_IMPORT
#endif
#else
#define MCD_EXPORT
#endif

#define MCD_VERSION_STR   "0.1.0"
/*
   MCD_VERSION is (major << 16) + (minor << 8) + patch.
*/
#define MCD_VERSION 0x000100
/*
   can be used like #if (MCD_VERSION >= MCD_VERSION_CHECK(4, 4, 0))
*/
#define MCD_VERSION_CHECK(major, minor, patch) ((major<<16)|(minor<<8)|(patch))

#define C_DISABLE_COPY(Class) Q_DISABLE_COPY(Class)

#define MCD_BEGIN_NAMESPACE QT_BEGIN_NAMESPACE
#define MCD_END_NAMESPACE QT_END_NAMESPACE

#define C_DECLARE_PRIVATE(Class) \
    inline Class##Private* p_func() { return reinterpret_cast<Class##Private *>(qGetPtrHelper(p_ptr)); } \
    inline const Class##Private* p_func() const { return reinterpret_cast<const Class##Private *>(qGetPtrHelper(p_ptr)); } \
    friend class Class##Private;

#define C_DECLARE_PUBLIC(Class)                                    \
    inline Class* b_func() { return static_cast<Class *>(b_ptr); } \
    inline const Class* b_func() const { return static_cast<const Class *>(b_ptr); } \
    friend class Class;

#define C_P(Class) Class##Private * const p = p_func()
#define C_B(Class) Class * const b = b_func()

/*
   Avoid "unused parameter" warnings
*/

#if defined(Q_CC_RVCT)
template <typename T>
inline void cUnused(T &x) { (void)x; }
#  define C_UNUSED(x) cUnused(x)
#else
#  define C_UNUSED(x) (void)x
#endif

#if defined(MCD_COORD_TYPE)
typedef MCD_COORD_TYPE creal;
#else
typedef double creal;
#endif

#define C_DECLARE_INITIALIZER(classname) friend struct classname##Initializer;\
    static void Init();

#define C_INITIALIZE_CLASS(classname) struct classname##Initializer\
{\
    classname##Initializer()\
    {\
        classname::Init();\
    }\
};\
static void __##classname##Init(){\
    classname##Initializer initializer;\
}\
Q_COREAPP_STARTUP_FUNCTION(__##classname##Init)

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
#  define MCD_FULLSCREEN_ONLY
#endif

#define C_REGISTER_QMLTYPE(uri, versionMajor, versionMinor, qmlName) static void __cRegisterQmlType__ ## qmlName()\
{\
    qmlRegisterType<qmlName>(uri, versionMajor, versionMinor, #qmlName);\
}\
Q_COREAPP_STARTUP_FUNCTION(__cRegisterQmlType__ ## qmlName)

#endif // CGLOBAL_H
