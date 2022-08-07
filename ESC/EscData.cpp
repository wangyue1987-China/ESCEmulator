#include "EscData.h"
#include <memory>
template <class T>
EscData<T>::EscData()
:_data(nullptr)
,_size(sizeof(T))
,_valid(false)
{
	_data = (uint8_t*)std::malloc(_size);
	if (_data)
	{
		_valid = true;
	}
}
template <class T>
EscData<T>::~EscData()
{
	std::free(_data);
	_data = nullptr;
}

template<class T>
T& EscData<T>::get() const
{
	return &T();
}



