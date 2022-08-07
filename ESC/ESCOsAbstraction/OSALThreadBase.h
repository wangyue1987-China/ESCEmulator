#pragma once
#include "EscTypes.h"
namespace OSAL
{
	enum class OSALThreadErrors
	{
		ThreadErrorOK,
		CreateThreadError,
		StartThreadError,
		StopThreadError,
		ResumeThreadError
	};

	class OSALThreadCallback
	{
	public:
		virtual int32_t threadCallback(void* param) = 0;
	};

	class OSALThreadBase
	{
	public:
		
		OSALThreadBase(int32_t Prio, OSALThreadCallback* callback,void* param, int32_t intervall);
		~OSALThreadBase();
		virtual OSALThreadErrors closeThread() = 0;
		virtual OSALThreadErrors startThread() = 0;

	protected:
		OSALThreadCallback* _callback;
		bool _stop;
		int32_t _threadCloseTimeOut;
		void* _callbackParam;
		int32_t _intervall;
		virtual OSALThreadErrors createThread() = 0;

	};
}

