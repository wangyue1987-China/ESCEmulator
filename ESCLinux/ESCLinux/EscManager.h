#pragma once
#include <list>
#include <string>
#include "OSALThread.h"
#include "OSALAdapter.h"
#include "ESC.h"

namespace ESC
{
	class EscManager:OSAL::OSALThreadCallback
	{
		typedef std::list<ESC::Esc*> EscList;

		EscList _esclist;
		OSAL::OSALThread _thread;
		OSAL::OSALAdapter _adapter;

		virtual int32_t callback(void* param);
		
	public:
		EscManager(const OSAL::OSALAdapter& adapter);
		~EscManager();
		void addEsc(Esc* escInstance);
		void removeEsc(Esc* escInstance);
		int32_t start();
		int32_t stop();
	};
}

