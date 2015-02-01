#ifndef CARDIRECTOR_GLOBAL_H
#define CARDIRECTOR_GLOBAL_H

#include <QtCore/qglobal.h>

#ifndef MCD_STATIC
#ifdef MCD_BUILD
#define MCD_EXPORT Q_DECL_EXPORT
#else
#define MCD_EXPORT Q_DECL_IMPORT
#endif
#else
#define MCD_EXPORT
#endif

#endif // CARDIRECTOR_GLOBAL_H
