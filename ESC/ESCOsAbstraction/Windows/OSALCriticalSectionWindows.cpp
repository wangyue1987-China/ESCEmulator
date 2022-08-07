#include "OSALCriticalSectionWindows.h"

OSAL::OSALCriticalSection::OSALCriticalSection()
	:OSALCriticalSectionBase()
, _criticalSection()
{
	//pthread_mutex_init(&_criticalSection, NULL);
}

OSAL::OSALCriticalSection::~OSALCriticalSection()
{
	//pthread_mutex_destroy(&_criticalSection);
}

void OSAL::OSALCriticalSection::enterCriticalSection()
{
	
}

void OSAL::OSALCriticalSection::leaveCriticalSection()
{
	
}
