#pragma once
#include "EscTypes.h"
#include <string>
#include <vector>
#include "EthercatFrame.h"


namespace OSAL
{
	class OSALAdapterCallback
	{
	public:
		virtual bool adapterCallback(void* param) = 0;
	};

	class OSALAdapterBase
	{
	protected:
		std::string _name;
		const uint16_t _ethertypeEthercat = 0x88A4;
		bool _valid;
		OSAL::OSALAdapterCallback* _Callback;
	public:
		OSALAdapterBase(const std::string& name,OSAL::OSALAdapterCallback* callback);
		OSALAdapterBase();
		
		~OSALAdapterBase();
		virtual int32_t AdapterOpen() = 0;
		virtual int32_t AdapterClose() = 0;
		virtual ESC::escSize_t adapterWrite(ESC::EthercatFrame& frame) = 0;
		virtual ESC::escSize_t adapterRead(ESC::EthercatFrame& frame) = 0;
		virtual ESC::escSize_t adapterWrite(uint8_t* data,ESC::escSize_t size) = 0;
		virtual ESC::escSize_t adapterRead(uint8_t* data, ESC::escSize_t size) = 0;

		bool valid() { return _valid; }
	};
}

