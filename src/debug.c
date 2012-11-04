#include "debug.h"

#ifndef NDEBUG
int dprintf(LPCTSTR szFormat, ...)
{
    int retval;
    TCHAR szBuffer[MAX_PATH];

    va_list vaList;
    va_start(vaList, szFormat);

    retval = wvsprintf(szBuffer, szFormat, vaList);
    OutputDebugString(szBuffer);

    va_end(vaList);
    return retval;
}

int dMsgBox(LPCTSTR szFormat, ...)
{
    int retval;
    TCHAR szBuffer[MAX_PATH];

    va_list vaList;
    va_start(vaList, szFormat);

    retval = wvsprintf(szBuffer, szFormat, vaList);
    MessageBox(GetActiveWindow(), szBuffer, _T("Debug"), MB_OK);

    va_end(vaList);
    return retval;
}
#endif
