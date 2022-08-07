#include "OSALFileWindows.h"

OSAL::OSALFile::OSALFile(std::string path)
	:OSALFileBase(path)
	, _stream()
{
}

OSAL::OSALFile::~OSALFile()
{
	
}

int32_t OSAL::OSALFile::readFile(void* data, size_t size)
{
	int32_t result = 0;
	_stream.open(_path, std::ios::in | std::ios::binary);
	_stream.read((char*)data, size);
	if (_stream.bad())
	{
		result = 1;
	}

	_stream.close();
	return result;
}

int32_t OSAL::OSALFile::writeFile(void* data, size_t size)
{
	int32_t result = 0;
	_stream.open(_path, std::ios::in | std::ios::trunc | std::ios::binary);
	_stream.write((char*)data, size);
	if (_stream.bad())
	{
		result = 1;
	}
	_stream.close();
	return result;
}
