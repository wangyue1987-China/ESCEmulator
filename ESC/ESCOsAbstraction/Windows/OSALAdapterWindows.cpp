#include "OSALAdapterWindows.h"
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

std::vector<std::string> OSAL::OSALAdapter::GetAdapters()
{
    std::vector<std::string> adapterList;
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

    pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(sizeof(IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
        return std::vector<std::string>();
    }
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        FREE(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            return std::vector<std::string>();
        }
    }
    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        while (pAdapter) 
        {
            if (pAdapter->Type == MIB_IF_TYPE_ETHERNET)
            {
                adapterList.push_back(std::string(pAdapter->AdapterName));
            }
            pAdapter = pAdapter->Next;
        }
    }
    else
    {
        adapterList = std::vector<std::string>();
    }
    if (pAdapterInfo)
        FREE(pAdapterInfo);
    return adapterList;
}

OSAL::OSALAdapter::OSALAdapter(const std::string& name,OSAL::OSALAdapterCallback* callback)
    :OSALAdapterBase(name, callback)
{
}

OSAL::OSALAdapter::OSALAdapter()
    : OSALAdapterBase()
{

}

int32_t OSAL::OSALAdapter::AdapterOpen()
{
    return int32_t();
}

int32_t OSAL::OSALAdapter::AdapterClose()
{
    return int32_t();
}

ESC::escSize_t OSAL::OSALAdapter::adapterWrite(ESC::EthercatFrame& frame)
{
    return int32_t();
}

ESC::escSize_t OSAL::OSALAdapter::adapterRead(ESC::EthercatFrame& frame)
{
    return int32_t();
}

ESC::escSize_t OSAL::OSALAdapter::adapterWrite(uint8_t* data, ESC::escSize_t size)
{
    return int32_t();
}

ESC::escSize_t OSAL::OSALAdapter::adapterRead(uint8_t* data, ESC::escSize_t size)
{
    return int32_t();
}
