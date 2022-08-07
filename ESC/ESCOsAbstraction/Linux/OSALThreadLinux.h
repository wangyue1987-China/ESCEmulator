#pragma once
#include <pthread.h>
#include "OSALThreadBase.h"
namespace OSAL
{
	class OSALThread :public OSALThreadBase
	{
		pthread_t _thread;

		static void* handler(void* param);
	public:
		OSALThread(int32_t Prio, OSALThreadCallback* callback, void* param, int32_t intervall)
			:OSALThreadBase(Prio, callback, param, intervall)
		{
			
		}

		virtual OSALThreadErrors closeThread();
		virtual OSALThreadErrors startThread();

	protected:
		virtual OSALThreadErrors createThread();
	};
}


