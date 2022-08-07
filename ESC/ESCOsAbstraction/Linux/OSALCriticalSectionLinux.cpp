#include "OSALCriticalSectionLinux.h"

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
	//pthread_mutex_lock(&_criticalSection);
}

void OSAL::OSALCriticalSection::leaveCriticalSection()
{
	//pthread_mutex_unlock(&_criticalSection);
}
