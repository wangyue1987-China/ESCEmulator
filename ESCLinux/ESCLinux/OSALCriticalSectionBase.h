#pragma once
namespace OSAL
{
	class OSALCriticalSectionBase
	{
	public:
		OSALCriticalSectionBase();
		virtual ~OSALCriticalSectionBase();
		virtual void enterCriticalSection() = 0;
		virtual void leaveCriticalSection() = 0;
	};
}

