#include "OSALThreadWindows.h"

OSAL::OSALThread::OSALThreadErrors OSAL::OSALThreadWindows::createThread()
{
    _handle = ::CreateThread(NULL, 0,  handler, this, CREATE_SUSPENDED, &_threadId);
    if (_handle == NULL)
    {
        return OSALThread::OSALThreadErrors::CreateThreadError;
    }
    return OSALThread::OSALThreadErrors::ThreadErrorOK;
}

DWORD __stdcall OSAL::OSALThreadWindows::handler(LPVOID param)
{
    OSALThreadWindows* th = (OSALThreadWindows*)param;
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

OSAL::OSALThread::OSALThreadErrors OSAL::OSALThreadWindows::closeThread()
{
    DWORD result;
    OSALThread::OSALThreadErrors threadResult = OSALThread::OSALThreadErrors::ThreadErrorOK;
    _stop = true;
    ::SetEvent(_handleIntervallEvent);
    result = ::WaitForSingleObject(_handle, _threadCloseTimeOut);
    if (result == WAIT_TIMEOUT)
    {
        ::TerminateThread(_handle,(int)OSALThread::OSALThreadErrors::StopThreadError);
        threadResult = OSALThread::OSALThreadErrors::StopThreadError;
    }
    ::CloseHandle(_handle);
    ::CloseHandle(_handleIntervallEvent);
    return threadResult;
}

OSAL::OSALThread::OSALThreadErrors OSAL::OSALThreadWindows::startThread()
{
    if (_handle == NULL)
    {
        return OSALThread::OSALThreadErrors::CreateThreadError;
    }
    if (ResumeThread(_handle) == (DWORD)-1)
    {
        return OSALThread::OSALThreadErrors::ResumeThreadError;
    }
    return OSALThread::OSALThreadErrors::ThreadErrorOK;
}



OSAL::OSALThread::OSALThreadErrors OSAL::OSALThreadWindows::lock()
{
    return OSALThread::OSALThreadErrors::ThreadErrorOK;
}

OSAL::OSALThread::OSALThreadErrors OSAL::OSALThreadWindows::unlock()
{
    return OSALThread::OSALThreadErrors::ThreadErrorOK;
}
