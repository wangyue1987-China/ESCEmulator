#include "EscManager.h"

ESC::EscManager::EscManager(const OSAL::OSALAdapter& adapter)
	:_esclist(EscList())
	, _thread(1, this, nullptr, 0)
	, _adapter(adapter)
{
}

ESC::EscManager::~EscManager()
{
}

int32_t ESC::EscManager::start()
{
	_thread.startThread();
	return int32_t();
}

int32_t ESC::EscManager::stop()
{
	return int32_t();
}

void ESC::EscManager::addEsc(Esc* escInstance)
{
	_esclist.push_back(escInstance);
}

void ESC::EscManager::removeEsc(Esc* escInstance)
{
	/*for (Esc aesc : _esclist)
	{
		_esclist.remove(escInstance);
	}*/
}

int32_t ESC::EscManager::callback(void* param)
{
	EthercatFrame ethframe;
	_adapter.adapterRead(ethframe);
	if ((ethframe.size() > 0) && (ethframe.size() != 0xFFFF))
	{
		for (Esc* aesc : _esclist)
		{
			aesc->processFrames(ethframe);
		}
		_adapter.adapterWrite(ethframe);
	}
	for (Esc* aesc : _esclist)
	{
		aesc->process();
	}
	
	return int32_t();
}
