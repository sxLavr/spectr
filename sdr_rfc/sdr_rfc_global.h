#ifndef SDR_RFC_GLOBAL_H
#define SDR_RFC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SDR_RFC_LIBRARY)
#  define SDR_RFCSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SDR_RFCSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SDR_RFC_GLOBAL_H
