#include "EthercatFrame.h"
#include <cstdlib>

using namespace ESC;

bool ESC::EthercatFrame::parse(uint8_t* frame, ESC::escSize_t size)
{
	bool next = false;
	ESC::escSize_t pduSize = 0;
	do
	{
		EthercatPDU pduTmp(frame + static_cast<ESC::escSize_t>(sizeof(EthernetHeader)) + pduSize, size);
		pduSize = static_cast<ESC::escSize_t>(static_cast<int32_t>(pduTmp.len()) + static_cast<int32_t>(pduTmp.pduOverHeadSize()));
		EthercatPDU* pdu = new EthercatPDU(pduTmp);
		_pduList.push_back(pdu);
		pdu->setSize(pduSize);
		next = pduTmp.next();
	} while (next);
	return false;
}

EthercatFrame::EthercatFrame()
	:_size(0)
	, _valid(false)
, _pduList(std::vector<EthercatPDU*>())
{
	_ethernetFrame.ethernetFrame = reinterpret_cast<uint8_t*>(std::malloc(_maxEthernetFrameSize));
	if (_ethernetFrame.ethernetFrame != nullptr)
	{
		_valid = true;
	}
}
EthercatFrame::~EthercatFrame()
{
	_pduList.clear();
	free(_ethernetFrame.ethernetFrame);
	_ethernetFrame.ethernetFrame = nullptr;
	_valid = false;
}

bool ESC::EthercatFrame::valid()
{
	return _valid;
}

EthercatPDU& ESC::EthercatFrame::getNextPDU()
{
	return *(new EthercatPDU());
}

EthercatPDU& ESC::EthercatFrame::getPDU(ESC::escSize_t index)
{
	if (_pduList.size() > index)
	{
		return *(_pduList[index]);
	}
	return *(new EthercatPDU());
}

ESC::escSize_t ESC::EthercatFrame::countOfPDUS()
{
	return static_cast<ESC::escSize_t>(_pduList.size());
}

EthercatPDU& EthercatFrame::operator[](ESC::escSize_t d)
{
	return getPDU(d);
}
