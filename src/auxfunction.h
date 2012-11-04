#ifndef _AUXFUCTION_H_
#define _AUXFUCTION_H_
#include "common.h"

void RemoveQuotes(LPTSTR szCommand);
void BeautyPath(LPTSTR szFullPath);
void DosDevicePathToWin32Path(LPTSTR szFullPath);
#endif
