#pragma once
#include "EscTypes.h"
#include "EthercatPDU.h"
#include <vector>

namespace ESC
{
	class EthercatFrame
	{
		enum class EthercatProtocollType
		{
			EthercatDLPDU = 0x01,
			NetworkVariable = 0x04,
			Mailbox = 0x05
		};

		typedef struct EthercatFrameHeader_t
		{
			uint16_t lenght : 11;
			uint8_t reserved1 : 1;
			uint8_t command : 4;
		}EthercatFrameHeader, * EthercatFrameHeaderP;

		typedef struct EthernetHeader_t
		{
			uint8_t destMAC[6];
			uint8_t srcMAC[6];
			uint16_t etherType;
			EthercatFrameHeader ethercatFrameHeader;
		}EthernetHeader, * EthernetHeaderP;

		typedef union
		{
			uint8_t* ethernetFrame;
			EthernetHeaderP ethernetHeader;
		}EthernetFrame;

		const ESC::escSize_t _maxEthernetFrameSize = 1518;
		EthernetFrame _ethernetFrame;
		escSize_t _size;
		bool _valid;
		std::vector<EthercatPDU*> _pduList;

		bool parse(uint8_t* frame, ESC::escSize_t size);
	public:
		EthercatFrame();
		~EthercatFrame();
		bool valid();
		EthercatPDU& getNextPDU();
		EthercatPDU& getPDU(ESC::escSize_t index);
		ESC::escSize_t countOfPDUS();
		EthercatPDU& operator[](ESC::escSize_t d);
		ESC::escSize_t size()
		{
			return _size;
		}
		uint8_t* pData()
		{
			return _ethernetFrame.ethernetFrame;
		}
		void setFrame(escSize_t size)
		{
			_size = size;
			parse(_ethernetFrame.ethernetFrame, size);
		}
		escSize_t maxEthFrameSize()
		{
			return _maxEthernetFrameSize;
		}
	};
}
