#pragma once
#include "EscTypes.h"

template <class T>
class EscData
{
	uint8_t* _data;
	uint16_t _size;
	bool _valid;
public:
	EscData();
	~EscData();

	T& get() const;
	

};



