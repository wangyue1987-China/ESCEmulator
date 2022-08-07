#pragma once
#include "OSALThreadBase.h"
#include <Windows.h>
namespace OSAL
{
	class OSALThread:OSALThreadBase
	{
		::HANDLE _handle;
		::DWORD _threadId;
		::HANDLE _handleIntervallEvent;
		static DWORD WINAPI handler(LPVOID param);
	public:
		OSALThread(int32_t Prio, OSALThreadCallback* callback, void* param, int32_t intervall)
			:OSALThreadBase(Prio, callback,param,intervall)
			,_handle(nullptr)
			,_threadId(0) 
			, _handleIntervallEvent(nullptr)
		{
			if (_intervall > 0)
			{
				_handleIntervallEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
			}
		};
		
		virtual OSALThreadErrors closeThread();
		virtual OSALThreadErrors startThread();

	protected:
		virtual OSALThreadErrors createThread();
	};
}


