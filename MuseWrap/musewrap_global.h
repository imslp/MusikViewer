//Copyright (C) 2018 Michail Montesatnos <musikviewer@imslp.org>
#ifndef MUSEWRAP_GLOBAL_H
#define MUSEWRAP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MUSEWRAP_LIBRARY)
#  define MUSEWRAPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MUSEWRAPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MUSEWRAP_GLOBAL_H
