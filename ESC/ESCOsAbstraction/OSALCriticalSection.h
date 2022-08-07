#pragma once
#ifdef _WIN32
#include "OSALCriticalSectionWindows.h"
#endif
#ifdef LINUX
#include "OSALCriticalSectionLinux.h"
#endif