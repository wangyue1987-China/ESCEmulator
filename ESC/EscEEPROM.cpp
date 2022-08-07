#include "EscEEPROM.h"
#include <string.h>

ESC::EscEEPROM::EscEEPROM(const std::string& name, const ESC::EscMemory& memory, ESC::escSize_t size)
	:_eePromMemory(0)
	, _registerEEPROMConfiguration(0)//Ethercat controls the Interface
	, _registerEEPROMPDIAccessState(0)//Ethercat controls the Interface
	, _registerEEPROMControlStatus({ 0 })
	, _registerEepromAddress(0)
	, _registerEepromData({ 0 })
	,_memory(memory)
	,_size(size)
	, _name(name)
	, _file("/home/pi/LEDStatusBarEEprom.bin")
	, _valid(true)
	, _lastCommand(0xFF)
	, _statemachine(0)
{
	_eePromMemory = (uint16_t*)malloc(_size);
	if (_eePromMemory == nullptr)
	{
		_valid = false;
		fprintf(stdout, "ESC::EscEEPROM::EscEEPROM : eePromMemory failed\n");
		return;
	}

	if (!_memory.valid())
	{
		fprintf(stdout, "ESC::EscEEPROM::EscEEPROM : _memory failed\n");
		_valid = false;
		return;
	}
	if (!_file.readFile((void*)_eePromMemory, _size))
	{
		fprintf(stdout, "ESC::EscEEPROM::EscEEPROM : readFile failed\n");
		_valid = false;
		return;
	}
}
ESC::EscEEPROM::EscEEPROM(const EscEEPROM& rhs)
	:_eePromMemory(rhs._eePromMemory)
	, _registerEEPROMConfiguration(rhs._registerEEPROMConfiguration)//Ethercat controls the Interface
	, _registerEEPROMPDIAccessState(rhs._registerEEPROMPDIAccessState)//Ethercat controls the Interface
	, _registerEEPROMControlStatus(rhs._registerEEPROMControlStatus)
	, _registerEepromAddress(rhs._registerEepromAddress)
	, _registerEepromData(rhs._registerEepromData)
	, _memory(rhs._memory)
	, _size(rhs._size)
	, _name(rhs._name)
	, _file("/home/pi/LEDStatusBarEEprom.bin")
	, _valid(rhs._valid)
	, _lastCommand(rhs._lastCommand)
	, _statemachine(rhs._statemachine)
{

}

ESC::EscEEPROM::~EscEEPROM()
{
	_file.writeFile(_eePromMemory, _size);
	if (_valid)
	{
		free(_eePromMemory);
	}
}

void ESC::EscEEPROM::process()
{
	ESC::EscRegisterAddress_t eepromAddresse = 0;
	uint32_t eepromDataBlock = 0;
	_registerEEPROMControlStatus.EEPROMControlStatusRaw = _memory.readEscRegister16(RegisterEEPROMControlStatusOffset);
	
		
	switch (_statemachine)
	{
	case 0://init wait for commandos
	{
		if (_registerEEPROMControlStatus.EEPROMControlStatus.CommandRegister != (uint8_t)ESCEepromCommands::NOP)
		{
			_statemachine = 100 + (int32_t)_registerEEPROMControlStatus.EEPROMControlStatus.CommandRegister;
		}
		break;
	}
	case 100 + (uint32_t)ESCEepromCommands::Read:
	{
		_registerEEPROMControlStatus.EEPROMControlStatus.Busy = 1;
		_memory.writeEscRegister16(RegisterEEPROMControlStatusOffset, _registerEEPROMControlStatus.EEPROMControlStatusRaw);
		eepromAddresse = _memory.readEscRegister16(RegisterEEPROMAddressOffset);
		fprintf(stdout, "ESC::EscEEPROM::process : eeprom read beginn eepromAddresse 0x%x\n", eepromAddresse);
		if (eepromAddresse > (_size / 2))
		{
			_registerEEPROMControlStatus.EEPROMControlStatus.Busy = 0;
			_registerEEPROMControlStatus.EEPROMControlStatus.ErrorAcknowledgeCommand = 1;
			_registerEEPROMControlStatus.EEPROMControlStatus.CommandRegister = 0;
			_memory.writeEscRegister16(RegisterEEPROMControlStatusOffset, _registerEEPROMControlStatus.EEPROMControlStatusRaw);
			fprintf(stdout, "ESC::EscEEPROM::process : eeprom read failed eepromAddresse 0x%x\n", eepromAddresse);
			_statemachine = 0;
			break;
		}
		
		readFromEeprom(eepromAddresse, 4, (uint8_t*)&eepromDataBlock);
		fprintf(stdout, "ESC::EscEEPROM::process : eeprom read  eepromAddresse 0x%x data 0x%x\n", eepromAddresse, eepromDataBlock);
		_memory.writeEscRegister32(RegisterEEPROMDataOffset, eepromDataBlock);
		_registerEEPROMControlStatus.EEPROMControlStatus.Busy = 0;
		_registerEEPROMControlStatus.EEPROMControlStatus.CommandRegister = 0;
		_memory.writeEscRegister16(RegisterEEPROMControlStatusOffset, _registerEEPROMControlStatus.EEPROMControlStatusRaw);
		_statemachine = 0;
		break;
	}
	case 100 + (uint32_t)ESCEepromCommands::Write:
	{
		break;
	}
	case 100 + (uint32_t)ESCEepromCommands::Reload:
	{
		break;
	}

	case 200 + (uint32_t)ESCEepromCommands::Read:
	{
		
		break;
	}

	}
}

void ESC::EscEEPROM::readFromEeprom(EscRegisterAddress_t adr, ESC::escSize_t size,uint8_t* data)
{
	memcpy(data ,&_eePromMemory[adr], size);
}

void ESC::EscEEPROM::writeToEeprom(EscRegisterAddress_t adr, uint16_t data)
{
	memcpy(&_eePromMemory[adr], &data, 2);
}

