#pragma once
#include <string>
#include "OsalFile.h"
#include "EscMemory.h"
#include "EscTypes.h"
namespace ESC
{
	class EscEEPROM
	{
		enum class ESCEepromCommands
		{
			NOP = 0,
			Read = 1,
			Write = 2,
			Reload = 4
		};
		typedef uint8_t RegisterEEPROMConfiguration_t;
		typedef uint8_t RegisterEEPROMPDIAccessState_t;
		struct RegisterEEPROMControlStatus_t
		{
			uint8_t ECATWriteEnable : 1;
			uint8_t reserved1 : 4;
			uint8_t EEPROMEmulation :  1;
			uint8_t SupportedNumberOfEEPROMReadBytes : 1;
			uint8_t SelectedEEPROMAlgorithm : 1;
			uint8_t CommandRegister : 3;
			uint8_t ChecksumErrorESCConfigurationArea : 1;
			uint8_t EEPROMLoadingStatus: 1;
			uint8_t ErrorAcknowledgeCommand : 1;
			uint8_t ErrorWriteEnable : 1;
			uint8_t Busy : 1;
		};
		union RegisterEEPROMControlStatus_u
		{
			RegisterEEPROMControlStatus_t EEPROMControlStatus;
			uint16_t EEPROMControlStatusRaw;
		};
		typedef uint16_t  RegisterEepromAddress_t;
		struct RegisterEepromData_t
		{
			uint16_t ReadWriteDataLower;
			uint64_t ReadWriteDataHigher : 48;
		};
		
		const ESC::EscRegisterAddress_t RegisterEEPROMConfigurationOffset = 0x0500;
		const ESC::EscRegisterAddress_t  RegisterEEPROMPDIAccessStateOffset = 0x0501;
		const ESC::EscRegisterAddress_t  RegisterEEPROMControlStatusOffset = 0x0502;
		const ESC::EscRegisterAddress_t RegisterEEPROMAddressOffset = 0x0504;
		const ESC::EscRegisterAddress_t RegisterEEPROMDataOffset = 0x0508;

		uint16_t* _eePromMemory;

		RegisterEEPROMConfiguration_t _registerEEPROMConfiguration;
		RegisterEEPROMPDIAccessState_t _registerEEPROMPDIAccessState;
		RegisterEEPROMControlStatus_u _registerEEPROMControlStatus;
		RegisterEepromAddress_t _registerEepromAddress;
		RegisterEepromData_t _registerEepromData;
		ESC::EscMemory _memory;
		ESC::escSize_t _size;
		std::string _name;
		OSAL::OSALFile _file;
		bool _valid;
		uint8_t _lastCommand;
		int32_t _statemachine;

		void readFromEeprom(EscRegisterAddress_t adr, ESC::escSize_t size, uint8_t* data);
		void writeToEeprom(EscRegisterAddress_t adr, uint16_t data);


	public:
		EscEEPROM(const std::string& name,const ESC::EscMemory& memory, ESC::escSize_t size);
		EscEEPROM(const EscEEPROM& rhs);
		~EscEEPROM();
		void process();

	};
}

