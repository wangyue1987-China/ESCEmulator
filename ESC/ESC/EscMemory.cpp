// ESC.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//


#include "EscMemory.h"
#include <cstdlib>
#include <string.h>
#include "EscTypes.h"

using namespace ESC;

EscMemory::EscMemory()
	:_memory()
	, _valid(false)
	, _criticalSection()
{
	_memory = (uint8_t*)std::malloc(_MemorySize);
	if (_memory)
	{
		_valid = true;
	}
	::memset(_memory, 0, _MemorySize);
	
}

ESC::EscMemory::EscMemory(const EscMemory& rhs)
	:_memory(rhs._memory)
	, _valid(rhs._valid)
	, _criticalSection(rhs._criticalSection)
{

}


EscMemory::~EscMemory()
{
	free(_memory);
	_memory = nullptr;
}

uint8_t EscMemory::readEscRegister8(EscRegisterAddress_t address)
{
	uint8_t val = 0;
	readEscRegister(address,sizeof(val),&val);
	return val;
}

uint16_t EscMemory::readEscRegister16(EscRegisterAddress_t address)
{
	uint16_t val = 0;
	readEscRegister(address, sizeof(val), (uint8_t*)&val);
	
	return val;
}

uint32_t EscMemory::readEscRegister32(EscRegisterAddress_t address)
{
	uint32_t val = 0;
	readEscRegister(address, sizeof(val), (uint8_t*)&val);
	return val;
}

int32_t EscMemory::readEscRegister(EscRegisterAddress_t address, ESC::escSize_t size, uint8_t * data)
{
	if (!data)
	{
		return -1;
	}
	if (!_valid)
	{
		return -1;
	}
	//fprintf(stdout, "EscMemory::readEscRegister register 0x%x  len %d\n", address, size);
	_criticalSection.enterCriticalSection();
	memcpy(data, &_memory[address], size);
	_criticalSection.leaveCriticalSection();
	return 0;
}

int32_t ESC::EscMemory::readOREscRegister(EscRegisterAddress_t address, ESC::escSize_t size, uint8_t* data)
{
	if (!data)
	{
		return -1;
	}
	if (!_valid)
	{
		return -1;
	}
	_criticalSection.enterCriticalSection();
	for (int32_t i = address; i < size + address; i++)
	{
		data[i] |= _memory[i];
	}
	_criticalSection.leaveCriticalSection();
	return int32_t();
}

int32_t ESC::EscMemory::readEscRegister(EscRegisterAddress_t address, EthercatPDUData& data)
{
	return readEscRegister(address, data.size(), data.data());
}

int32_t ESC::EscMemory::readOREscRegister(EscRegisterAddress_t address, EthercatPDUData& data)
{
	
	return readOREscRegister(address, data.size(), data.data());
}

void EscMemory::writeEscRegister8(EscRegisterAddress_t address, uint8_t data)
{
	writeEscRegister(address, sizeof(data), (uint8_t*)&data);
}

void EscMemory::writeEscRegister16(EscRegisterAddress_t address, uint16_t data)
{
	writeEscRegister(address, sizeof(data), (uint8_t*)&data);
}

void EscMemory::writeEscRegister32(EscRegisterAddress_t address, uint32_t data)
{
	writeEscRegister(address, sizeof(data), (uint8_t*)&data);
}

int32_t EscMemory::writeEscRegister(EscRegisterAddress_t address, ESC::escSize_t size, uint8_t * data)
{
	if (!data)
	{
		return -1;
	}
	if (!_valid)
	{
		return -1;
	}
	//fprintf(stdout, "EscMemory::writeEscRegister register 0x%x  len %d\n", address, size);
	_criticalSection.enterCriticalSection();
	memcpy(&_memory[address],data, size);
	if (address == 0x120)
	{
		memcpy(&_memory[0x130], data, size);
	}
	_criticalSection.leaveCriticalSection();
	
	return 0;
}
int32_t ESC::EscMemory::writeOREscRegister(EscRegisterAddress_t address, ESC::escSize_t size, uint8_t* data)
{
	if (!data)
	{
		return -1;
	}
	if (!_valid)
	{
		return -1;
	}
	_criticalSection.enterCriticalSection();
	for (ESC::escSize_t i = address; i < (size + address); i++)
	{
		_memory[i] |= data[i];
	}
	_criticalSection.leaveCriticalSection();
	return int32_t();
}

int32_t ESC::EscMemory::writeOREscRegister(EscRegisterAddress_t address, EthercatPDUData& data)
{
	return writeOREscRegister(address, data.size(), data.data());
}

int32_t ESC::EscMemory::writeEscRegister(EscRegisterAddress_t address, EthercatPDUData& data)
{
	writeEscRegister(address, data.size(), data.data());
	return int32_t();
}
