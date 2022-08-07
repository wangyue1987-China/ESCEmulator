#include "OSALAdapterBase.h"

OSAL::OSALAdapterBase::OSALAdapterBase(const std::string& name)
	:_name(name)
	, _valid(true)
{
}

OSAL::OSALAdapterBase::OSALAdapterBase()
	: _name(std::string(""))
	, _valid(false)
{
}

OSAL::OSALAdapterBase::~OSALAdapterBase()
{
}


