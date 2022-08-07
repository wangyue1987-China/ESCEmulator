#pragma once
#include "OSALFileBase.h"
#include <fstream>
namespace OSAL
{
	class OSALFile : public OSALFileBase
	{
		std::fstream _stream;
	public:
		OSALFile(std::string path);
		~OSALFile();
		virtual int32_t readFile(void* data, size_t size);
		virtual int32_t writeFile(void* data, size_t size);
	};
}

