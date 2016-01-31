#ifndef QHIDAPI_GLOBAL_H
#define QHIDAPI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QHIDAPI_LIBRARY)
#  define QHIDAPISHARED_EXPORT Q_DECL_EXPORT
#else
#  define QHIDAPISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QHIDAPI_GLOBAL_H
