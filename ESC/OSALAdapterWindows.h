#pragma once
#include "OSALAdapterBase.h"
namespace OSAL
{
	class OSALAdapter :OSALAdapterBase
	{
	public:
		static std::vector<std::string> GetAdapters();
		virtual int32_t AdapterOpen();
		virtual int32_t AdapterClose();
		virtual int32_t adapterWrite(ESC::EthercatFrame& frame);
		virtual int32_t adapterRead(ESC::EthercatFrame& frame);
	};
}

