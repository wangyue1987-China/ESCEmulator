#pragma once
#include "OSALCriticalSectionBase.h"
#include <pthread.h>
namespace OSAL
{
	class OSALCriticalSection: public OSALCriticalSectionBase
	{
		pthread_mutex_t _criticalSection;
	public:
		OSALCriticalSection();
		~OSALCriticalSection();
		virtual void enterCriticalSection();
		virtual void leaveCriticalSection();
	};
}
