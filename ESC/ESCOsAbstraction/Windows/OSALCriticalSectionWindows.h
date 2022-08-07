#pragma once
#include "OSALCriticalSectionBase.h"
#include <Windows.h>

namespace OSAL
{
	class OSALCriticalSection: public OSALCriticalSectionBase
	{
		LPCRITICAL_SECTION _criticalSection;
	public:
		OSALCriticalSection();
		~OSALCriticalSection();
		virtual void enterCriticalSection();
		virtual void leaveCriticalSection();
	};
}
