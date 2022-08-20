#include "EscManager.h"

ESC::EscManager::EscManager(const OSAL::OSALAdapter& adapter)
	:_esclist(EscList())
	, _thread(1, this, nullptr, 0)
	, _adapter(adapter)
{
}

ESC::EscManager::EscManager()
	:_esclist(EscList())
	, _thread(1, this, nullptr, 0)
	, _adapter(OSAL::OSALAdapter())
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

void ESC::EscManager::addEsc(const std::string EsiFilePath)
{
	
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

void ESC::EscManager::ProcessFrame(EthercatFrame& ethframe)
{
	if ((ethframe.size() > 0) && (ethframe.size() != 0xFFFF))
	{
		for (Esc* aesc : _esclist)
		{
			aesc->processFrames(ethframe);
		}
		_adapter.adapterWrite(ethframe);
	}
}

int32_t ESC::EscManager::threadCallback(void* param)
{
	EthercatFrame ethframe;
	if(_adapter.valid())
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

bool ESC::EscManager::adapterCallback(void* param)
{
	return false;
}
