#include "OSALAdapterBase.h"

OSAL::OSALAdapterBase::OSALAdapterBase(const std::string& name, OSAL::OSALAdapterCallback* callback)
	:_name(name)
	, _valid(true)
	,_Callback(callback)
{
}

OSAL::OSALAdapterBase::OSALAdapterBase()
	: _name(std::string(""))
	, _valid(false)
	, _Callback(nullptr)
{
}

OSAL::OSALAdapterBase::~OSALAdapterBase()
{
}


