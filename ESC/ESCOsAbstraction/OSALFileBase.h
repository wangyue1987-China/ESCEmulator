#pragma once
#include <string>
#include "EscTypes.h"

namespace OSAL
{
	class OSALFileBase
	{
	protected:
		std::string _path;
	public:
		OSALFileBase(std::string path);
		virtual ~OSALFileBase();
		virtual int32_t readFile(void* data,size_t size) = 0;
		virtual int32_t writeFile(void* data, size_t size) = 0;
	};
}


