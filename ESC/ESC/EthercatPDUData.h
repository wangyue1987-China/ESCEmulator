#pragma once
#include "EscTypes.h"

class EthercatPDUData
{
	uint8_t* _data;
	ESC::escSize_t _size;
public:
	EthercatPDUData(uint8_t* data, ESC::escSize_t size)
	:_data(data)
	, _size(size)
	{
		
	}
	EthercatPDUData()
		:_data(nullptr)
		,_size(0)
	{
		
	}
	uint8_t* data()
	{
		return _data;
	}
	ESC::escSize_t	size()
	{
		return _size;
	}
	void data(uint8_t* data)
	{
		_data = data;
	}
	void size(ESC::escSize_t size)
	{
		_size = size;
	}
};

