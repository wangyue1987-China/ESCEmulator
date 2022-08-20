#pragma once
#include <list>
#include <string>
#include "OSALThread.h"
#include "OSALAdapter.h"
#include "ESC.h"

namespace ESC
{
	class EscManager:public OSAL::OSALThreadCallback, public OSAL::OSALAdapterCallback
	{
		typedef std::list<ESC::Esc*> EscList;

		EscList _esclist;
		OSAL::OSALThread _thread;
		OSAL::OSALAdapter _adapter;

		virtual int32_t threadCallback(void* param);
		virtual bool adapterCallback(void* param);
		
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

