#pragma once
#include "EthercatFrame.h"
#include "EthercatPDU.h"
#include "EscMemory.h"
#include "OSALAdapter.h"
#include "EscEEPROM.h"
#include <string>

namespace ESC
{
	class Esc
	{
	public:
		Esc(const std::string& name, const std::string& configFilePath);
		~Esc();
		int32_t start();
		void stop();
		void process();
		void processFrames(EthercatFrame& ethframe);

		const std::string& name()
		{
			return _name;
		}

		bool operator == (const Esc& rhs);

	private:
		EscMemory _memory;
		std::string _name;
		EscEEPROM _eeprom;
		std::string _configFilePath;

		void setup();
		void processPDUs(EthercatPDU& pdu);
		void cmdNOP(EthercatPDU& pdu);
		//Auto Increment Read
		void cmdAPRD(EthercatPDU& pdu);
		void cmdAPWR(EthercatPDU& pdu);
		void cmdAPRW(EthercatPDU& pdu);

		//Configured Address Read
		void cmdFPRD(EthercatPDU& pdu);
		void cmdFPWR(EthercatPDU& pdu);
		void cmdFPRW(EthercatPDU& pdu);

		//Broadcast Read
		void cmdBRD(EthercatPDU& pdu);
		void cmdBWR(EthercatPDU& pdu);
		void cmdBRW(EthercatPDU& pdu);
		//Logical Memory Read
		void cmdLRD(EthercatPDU& pdu);
		void cmdLWR(EthercatPDU& pdu);
		void cmdLRW(EthercatPDU& pdu);
		void cmdARMW(EthercatPDU& pdu);
		void cmdFRMW(EthercatPDU& pdu);
	};
}

