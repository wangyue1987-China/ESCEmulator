#include "EthercatPDU.h"
using namespace ESC;


EthercatPDU::EthercatPDU(uint8_t* pdu, ESC::escSize_t size)
:_pdu()
 ,_pduData(EthercatPDUData(0,0))
,_wkz(nullptr)
,_size(size)
,_valid(true)
{
	if (pdu == nullptr)
	{
		_valid = false;
		return;
	}
	_pdu.pdu = pdu;
	_pduData.data(pdu + sizeof(EthercatPDUHeader_t));
	_pduData.size(_pdu.pduHeader->len);
	_wkz = (uint16_t*)(pdu + sizeof(EthercatPDUHeader_t) + _pdu.pduHeader->len);
}

int32_t ESC::EthercatPDU::wkzOffset()
{
	return int32_t();
}

EthercatPDU::EthercatPDU()
	:_pdu()
	, _pduData(EthercatPDUData(0, 0))
	, _wkz(nullptr)
	, _size(0)
{
	_valid = false;
}

EthercatPDU::~EthercatPDU()
{
}

EthercatPDU::EthercatCMD EthercatPDU::cmd() const
{
	if (!_valid || (_pdu.pdu == nullptr))
	{
		return EthercatPDU::EthercatCMD::NOP;
	}
	return (EthercatCMD)_pdu.pduHeader->command;
}
uint8_t EthercatPDU::idx() const
{
	if (!_valid || (_pdu.pdu == nullptr))
	{
		return 0;
	}
	return _pdu.pduHeader->idx;
}
uint16_t EthercatPDU::adp() const
{
	if (!_valid || (_pdu.pdu == nullptr))
	{
		return 0;
	}
	return _pdu.pduHeader->adp;
}
uint16_t EthercatPDU::ado() const
{
	if (!_valid || (_pdu.pdu == nullptr))
	{
		return 0;
	}
	return _pdu.pduHeader->ado;
}
ESC::escSize_t EthercatPDU::len() const
{
	return _pdu.pduHeader->len;
}
bool EthercatPDU::circulatingframe() const
{
	if (!_valid || (_pdu.pdu == nullptr))
	{
		return 0;
	}
	return _pdu.pduHeader->circulatingframe;
}
bool EthercatPDU::next() const
{
	if (!_valid || (_pdu.pdu == nullptr))
	{
		return 0;
	}
	return _pdu.pduHeader->next;
}
uint16_t EthercatPDU::irq() const
{
	if (!_valid || (_pdu.pdu == nullptr))
	{
		return 0;
	}
	return _pdu.pduHeader->irq;
}

bool ESC::EthercatPDU::valid() const
{
	if (!_valid || (_pdu.pdu == nullptr))
	{
		return 0;
	}
	return _valid;
}

void ESC::EthercatPDU::incrementWkz() const
{
	(*_wkz)++;
}

void ESC::EthercatPDU::incrementAdp() const
{
	_pdu.pduHeader->adp++;
}
