#pragma once
#include "EscTypes.h"
#include "EthercatPDUData.h"

namespace ESC
{
	class EthercatPDU
	{
	public:
		enum class EthercatCMD
		{
			NOP,
			APRD,
			APWR,
			APRW,
			FPRD,
			FPWR,
			FPRW,
			BRD,
			BWR,
			BRW,
			LRD,
			LWR,
			LRW,
			ARMW,
			FRMW
		};
	private:
		const ESC::escSize_t WKZSize = 2;
		typedef struct EthercatPDUHeader_t
		{
			uint8_t command:8;
			uint8_t idx : 8;
			uint16_t adp : 16;
			uint16_t ado : 16;
			uint16_t len : 11;
			uint8_t reserve1 : 3;
			uint8_t circulatingframe : 1;
			uint8_t next : 1;
			uint16_t irq : 16;
		}EthercatPDUHeader;

		typedef union
		{
			uint8_t* pdu;
			EthercatPDUHeader_t* pduHeader;
		}Pdu,*PPdu;
		
		Pdu _pdu;
		EthercatPDUData _pduData;
		uint16_t* _wkz;
		escSize_t _size;
		bool _valid;
		int32_t wkzOffset();
	public:
		EthercatPDU();
		EthercatPDU(uint8_t* pdu, ESC::escSize_t size);
		~EthercatPDU();

		EthercatCMD cmd() const;
		uint8_t idx() const;
		uint16_t adp() const;
		uint16_t ado() const;
		ESC::escSize_t len() const;
		bool circulatingframe() const;
		bool next() const;
		uint16_t irq() const;
		bool valid() const;
		void setSize(ESC::escSize_t size)
		{
			_size = size;
		}
		void incrementWkz() const;
		void incrementAdp() const;
		EthercatPDUData& pduData()
		{
			return _pduData;
		}
		escSize_t pduOverHeadSize() const
		{
			//warning : conversion from ‘int’ to ‘ESC::escSize_t’ {aka ‘short unsigned int’} may change value [-Wconversion]
			return static_cast<ESC::escSize_t>(static_cast<int32_t>(sizeof(EthercatPDUHeader)) + static_cast<int32_t>(WKZSize));
		}
	};
}