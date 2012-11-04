#ifndef _STATUSBAR_H_
#define _STATUSBAR_H_
#include "common.h"

enum StatusBarColNames
{
    ProcessCount,
    AvailPhysMemPercent,
    AvailPhysMem,
};

void InitStatusBar(void);
void UpdateStatusBar(void);
#endif
