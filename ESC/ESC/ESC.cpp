#include "ESC.h"
#include "EthercatPDU.h"
using namespace ESC;

ESC::Esc::Esc(const std::string& name, const std::string& configFilePath)
	: _memory()
	, _name(name)
	, _eeprom()
	, _configFilePath(configFilePath)
	
{
	
}

ESC::Esc::~Esc()
{
	
}

int32_t ESC::Esc::start()
{
	return 0;
}

void ESC::Esc::stop()
{
}

void ESC::Esc::processFrames(EthercatFrame& ethframe)
{
	int32_t cntPdu = ethframe.countOfPDUS();
	for(ESC::escSize_t i = 0; i < cntPdu;i++)
	{
		processPDUs(ethframe[i]);
	}
	
}

bool ESC::Esc::operator==(const Esc& rhs)
{
	return (_name.compare(rhs._name) == 0);
}

void ESC::Esc::process()
{
	_eeprom.process();
}

void ESC::Esc::setup()
{
	_memory.writeEscRegister16(0x130, 0x0100);
}

void Esc::processPDUs(EthercatPDU& pdu)
{
	switch (pdu.cmd())
	{
	case EthercatPDU::EthercatCMD::NOP:
	{
		cmdNOP(pdu);
		break;
	}
	case EthercatPDU::EthercatCMD::APRD:
	{
		cmdAPRD(pdu);
		break;
	}
	case EthercatPDU::EthercatCMD::APWR:
	{
		cmdAPWR(pdu);
		break;
	}
	case EthercatPDU::EthercatCMD::APRW:
	{
		cmdAPRW(pdu);
		break;
	}
	case EthercatPDU::EthercatCMD::FPRD:
	{
		cmdFPRD(pdu);
		break;
	}
	case EthercatPDU::EthercatCMD::FPWR:
	{
		cmdFPWR(pdu);
		break;
	}
	case EthercatPDU::EthercatCMD::FPRW:
	{
		cmdFPRW(pdu);
		break;
	}
	case EthercatPDU::EthercatCMD::BRD:
	{
		cmdBRD(pdu);
		break;
	}
	case EthercatPDU::EthercatCMD::BWR:
	{
		cmdBWR(pdu);
		break;
	}
	case EthercatPDU::EthercatCMD::BRW:
	{
		cmdBRW(pdu);
		break;
	}
	case EthercatPDU::EthercatCMD::LRD:
	{
		cmdLRD(pdu);
		break;
	}
	case EthercatPDU::EthercatCMD::LWR:
	{
		cmdLWR(pdu);
		break;
	}
	case EthercatPDU::EthercatCMD::LRW:
	{
		cmdLRW(pdu);
		break;
	}
	case EthercatPDU::EthercatCMD::ARMW:
	{
		cmdARMW(pdu);
		break;
	}
	case EthercatPDU::EthercatCMD::FRMW:
	{
		cmdFRMW(pdu);
		break;
	}
	}
}

void Esc::cmdNOP(EthercatPDU& pdu)
{
	pdu.adp();
}
//Auto Increment Read
void Esc::cmdAPRD(EthercatPDU& pdu)
{
	if (pdu.adp() == 0)
	{
		_memory.readEscRegister(pdu.ado(),pdu.pduData());
		
		if (pdu.ado() == 0x0010)
		{
			fprintf(stdout, "read Configured Address %d\n", *((uint16_t*)pdu.pduData().data()));
		}
		if (pdu.ado() == 0x502)
		{
			fprintf(stdout, "read EEPROM %d\n", *((uint16_t*)pdu.pduData().data()));
		}
	}
	pdu.incrementWkz();
	pdu.incrementAdp();
	
}

void Esc::cmdAPWR(EthercatPDU& pdu)
{
	if (pdu.adp() == 0)
	{
		if (pdu.ado() == 0x0010)
		{
			fprintf(stdout, "Set Configured Address %d\n", *((uint16_t*)pdu.pduData().data()));
		}
		if (pdu.ado() == 0x502)
		{
			fprintf(stdout, "Set EEPROM %d\n", *((uint16_t*)pdu.pduData().data()));
		}
		_memory.writeEscRegister(pdu.ado(), pdu.pduData());
	}
	
	pdu.incrementAdp();
	pdu.incrementWkz();
	
}

void Esc::cmdAPRW(EthercatPDU& pdu)
{
	if (pdu.adp() == 0)
	{
		EthercatPDUData dataTmp(pdu.pduData());
		_memory.readEscRegister(pdu.ado(), pdu.pduData());
		_memory.writeEscRegister(pdu.ado(), dataTmp);
	}
	pdu.incrementWkz();
	pdu.incrementAdp();
}
//Configured Address Read
void Esc::cmdFPRD(EthercatPDU& pdu)
{
	uint16_t adr = _memory.readEscRegister16(0x10);
	
	if (pdu.adp() == adr)
	{
		_memory.readEscRegister(pdu.ado(), pdu.pduData());
	}
	pdu.incrementWkz();
}

void Esc::cmdFPWR(EthercatPDU& pdu)
{
	uint16_t adr = _memory.readEscRegister16(0x10);
	if (pdu.adp() == adr)
	{
		_memory.writeEscRegister(pdu.ado(), pdu.pduData());
	}
	pdu.incrementWkz();
}

void Esc::cmdFPRW(EthercatPDU& pdu)
{
	uint16_t adr = _memory.readEscRegister16(0x10);
	if (pdu.adp() == adr)
	{
		EthercatPDUData dataTmp(pdu.pduData());
		_memory.readEscRegister(pdu.ado(), pdu.pduData());
		_memory.writeEscRegister(pdu.ado(), dataTmp);
	}
	pdu.incrementWkz();
}
//Broadcast Read
void Esc::cmdBRD(EthercatPDU& pdu)
{
	pdu.incrementWkz();
	pdu.incrementAdp();
	_memory.readOREscRegister(pdu.ado(), pdu.pduData());
}

void Esc::cmdBWR(EthercatPDU& pdu)
{
	pdu.incrementWkz();
	pdu.incrementAdp();
	_memory.writeOREscRegister(pdu.ado(), pdu.pduData());

}

void Esc::cmdBRW(EthercatPDU& pdu)
{
	pdu.incrementWkz();
	pdu.incrementAdp();
	EthercatPDUData dataTmp(pdu.pduData());
	_memory.readOREscRegister(pdu.ado(), pdu.pduData());
	_memory.writeOREscRegister(pdu.ado(), dataTmp);
}
//Logical Memory Read
void Esc::cmdLRD(EthercatPDU& pdu)
{

}

void Esc::cmdLWR(EthercatPDU& pdu)
{

}

void Esc::cmdLRW(EthercatPDU& pdu)
{

}

void Esc::cmdARMW(EthercatPDU& pdu)
{

}

void Esc::cmdFRMW(EthercatPDU& pdu)
{

}


	
