#include "OSALThreadLinux.h"
#include <unistd.h>

void* OSAL::OSALThread::handler(void* param)
{
    OSALThread* th = (OSALThread*)param;
    while (!th->_stop)
    {
       th->_callback->callback(th->_callbackParam);
       if (th->_intervall > 0)
       {
           ::usleep(1000 * th->_intervall);
       }
    }
    return nullptr;
}

OSAL::OSALThreadErrors OSAL::OSALThread::closeThread()
{
    void* joinResult = nullptr;
    int res = pthread_join(_thread, &joinResult);
    if (res != 0)
    {
        pthread_cancel(_thread);
        return OSALThreadErrors::StopThreadError;
    }
    return OSALThreadErrors::ThreadErrorOK;
}

OSAL::OSALThreadErrors OSAL::OSALThread::startThread()
{
    return createThread();
}

OSAL::OSALThreadErrors OSAL::OSALThread::createThread()
{
    int result = pthread_create(&_thread, NULL, handler, this);
    if (result != 0)
    {
        return OSALThreadErrors::StartThreadError;
    }
    return OSALThreadErrors::ThreadErrorOK;
}
