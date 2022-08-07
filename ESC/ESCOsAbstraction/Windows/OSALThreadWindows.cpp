#include "OSALThreadWindows.h"

OSAL::OSALThreadErrors OSAL::OSALThread::createThread()
{
    _handle = ::CreateThread(NULL, 0,  handler, this, CREATE_SUSPENDED, &_threadId);
    if (_handle == NULL)
    {
        return OSAL::OSALThreadErrors::CreateThreadError;
    }
    return OSAL::OSALThreadErrors::ThreadErrorOK;
}

DWORD __stdcall OSAL::OSALThread::handler(LPVOID param)
{
    OSALThread* th = (OSALThread*)param;
    while (!th->_stop)
    {
        if (th->_handleIntervallEvent)
        {
            if (WaitForSingleObject(th->_handleIntervallEvent, th->_intervall) == WAIT_TIMEOUT)
            {
                th->_callback->callback(th->_callbackParam);
            }
            else
            {
                return 0;
            }
        }

       
    }
    
    return 0;
}

OSAL::OSALThreadErrors OSAL::OSALThread::closeThread()
{
    DWORD result;
    OSAL::OSALThreadErrors threadResult = OSAL::OSALThreadErrors::ThreadErrorOK;
    _stop = true;
    ::SetEvent(_handleIntervallEvent);
    result = ::WaitForSingleObject(_handle, _threadCloseTimeOut);
    if (result == WAIT_TIMEOUT)
    {
        ::TerminateThread(_handle,(int)OSAL::OSALThreadErrors::StopThreadError);
        threadResult = OSAL::OSALThreadErrors::StopThreadError;
    }
    ::CloseHandle(_handle);
    ::CloseHandle(_handleIntervallEvent);
    return threadResult;
}

OSAL::OSALThreadErrors OSAL::OSALThread::startThread()
{
    if (_handle == NULL)
    {
        return OSAL::OSALThreadErrors::CreateThreadError;
    }
    if (ResumeThread(_handle) == (DWORD)-1)
    {
        return OSAL::OSALThreadErrors::ResumeThreadError;
    }
    return OSAL::OSALThreadErrors::ThreadErrorOK;
}

