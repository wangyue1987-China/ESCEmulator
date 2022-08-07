#include "OSALThreadBase.h"

OSAL::OSALThreadBase::OSALThreadBase(int32_t Prio, OSALThreadCallback* callback,void* param,int32_t intervall)
: _callback(callback)
,_stop(false)
,_callbackParam(param)
,_intervall(intervall)
{
}

OSAL::OSALThreadBase::~OSALThreadBase()
{
}
