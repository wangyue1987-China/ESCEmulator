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

		virtual int32_t threadCallback(void* param);
		
	public:
		EscManager(const OSAL::OSALAdapter& adapter);
		EscManager();
		~EscManager();
		void addEsc(const std::string EsiFilePath);
		void addEsc(Esc* escInstance);
		void removeEsc(Esc* escInstance);
		void ProcessFrame(EthercatFrame& ethframe);
		int32_t start();
		int32_t stop();
	};
}

