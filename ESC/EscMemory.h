#pragma once
#include "EscTypes.h"
#include "EthercatPDUData.h"
#include "OSALCriticalSection.h"
namespace ESC
{
	typedef uint16_t EscRegisterAddress_t;

	class EscMemory
	{
		const ESC::escSize_t _MemorySize = (ESC::escSize_t)((1024 * 64) - 1);
		uint8_t* _memory;
		bool _valid;
		OSAL::OSALCriticalSection _criticalSection;
	public:
		EscMemory();
		EscMemory(const EscMemory& rhs);
		~EscMemory();
		bool valid()
		{
			return _valid;
		}
		uint8_t readEscRegister8(EscRegisterAddress_t address);
		uint16_t readEscRegister16(EscRegisterAddress_t address);
		uint32_t readEscRegister32(EscRegisterAddress_t address);
		int32_t readEscRegister(EscRegisterAddress_t address, ESC::escSize_t size, uint8_t* data);
		int32_t readOREscRegister(EscRegisterAddress_t address, ESC::escSize_t size, uint8_t* data);
		int32_t readEscRegister(EscRegisterAddress_t address, EthercatPDUData& data);
		int32_t readOREscRegister(EscRegisterAddress_t address, EthercatPDUData& data);
		void writeEscRegister8(EscRegisterAddress_t address, uint8_t data);
		void writeEscRegister16(EscRegisterAddress_t address, uint16_t data);
		void writeEscRegister32(EscRegisterAddress_t address, uint32_t data);
		int32_t writeEscRegister(EscRegisterAddress_t address, ESC::escSize_t size, uint8_t* data);
		int32_t writeOREscRegister(EscRegisterAddress_t address, ESC::escSize_t size, uint8_t* data);
		int32_t writeEscRegister(EscRegisterAddress_t address, EthercatPDUData& data);
		int32_t writeOREscRegister(EscRegisterAddress_t address, EthercatPDUData& data);
	};
}

