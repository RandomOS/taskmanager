#ifndef _DEBUG_H_
#define _DEBUG_H_
#include "common.h"

#ifndef NDEBUG
#define ODS(sz) OutputDebugString(sz)
#else
#define ODS(sz)
#endif

#ifndef NDEBUG
int dprintf(LPCTSTR szFormat, ...);
int dMsgBox(LPCTSTR szFormat, ...);
#else
#ifdef _MSC_VER
#define dprintf(x)
#define dMsgBox(x)
#else // for mingw
#define dprintf(x, ...)
#define dMsgBox(x, ...)
#endif
#endif
#endif
