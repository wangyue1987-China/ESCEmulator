#include "OSALAdapterLinux.h"
#include <net/if.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <netpacket/packet.h>

OSAL::OSALAdapter::OSALAdapter(const std::string& name)
    :OSALAdapterBase(name)
    , _socket(-1)
{
}

const std::vector<std::string> OSAL::OSALAdapter::GetAdapters()
{
    struct if_nameindex* ids;
    std::vector<std::string> list;
    ids = if_nameindex();
    for (int i = 0; ids[i].if_index != 0; i++)
    {
        if (ids[i].if_name)
        {
            std::string adapter = std::string(ids[i].if_name);
            list.push_back(adapter);
        }
       
    }
    return std::vector<std::string>(list);
}

int32_t OSAL::OSALAdapter::AdapterOpen()
{
    struct ::timeval timeout;
    struct ::ifreq ifr;
    struct ::sockaddr_ll sll;
    int result = -1;
    int ifindex = 0;

    _socket = ::socket(PF_PACKET, SOCK_RAW, htons(_ethertypeEthercat));
    if (_socket < 0)
    {
        fprintf(stderr, "socket() failed: %s\n", strerror(errno));
        return _socket;
    }

    timeout.tv_sec = 0;
    timeout.tv_usec = 1;
    result = ::setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if (result < 0)
    {
        return result;
    }
    result = ::setsockopt(_socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    if (result < 0)
    {
        return result;
    }
    int i = 1;
    result = ::setsockopt(_socket, SOL_SOCKET, SO_DONTROUTE, &i, sizeof(i));
    if (result < 0)
    {
        return result;
    }
    /* connect socket to NIC by name */
    ::strcpy(ifr.ifr_name, _name.c_str());
    result = ::ioctl(_socket, SIOCGIFINDEX, &ifr);
    if (result < 0)
    {
        return result;
    }
    ifindex = ifr.ifr_ifindex;
    ::strcpy(ifr.ifr_name, _name.c_str());
    ifr.ifr_flags = 0;
    /* reset flags of NIC interface */
    result = ::ioctl(_socket, SIOCGIFFLAGS, &ifr);
    if (result < 0)
    {
        return result;
    }
    /* set flags of NIC interface, here promiscuous and broadcast */
    ifr.ifr_flags = ifr.ifr_flags | IFF_PROMISC | IFF_BROADCAST;
    result = ::ioctl(_socket, SIOCSIFFLAGS, &ifr);
    if (result < 0)
    {
        return result;
    }
    /* bind socket to protocol, in this case RAW EtherCAT */
    sll.sll_family = AF_PACKET;
    sll.sll_ifindex = ifindex;
    sll.sll_protocol = ::htons(_ethertypeEthercat);
    result = ::bind(_socket, (struct sockaddr*)&sll, sizeof(sll));
    return result;
}

int32_t OSAL::OSALAdapter::AdapterClose()
{
    return ::close(_socket);
}

ESC::escSize_t OSAL::OSALAdapter::adapterWrite(uint8_t* data, ESC::escSize_t size)
{
    return static_cast<ESC::escSize_t>(::send(_socket, data, static_cast<std::size_t>(size), 0));
}

ESC::escSize_t OSAL::OSALAdapter::adapterRead(uint8_t* data, ESC::escSize_t size)
{
    return static_cast<ESC::escSize_t>(::recv(_socket, data, static_cast<std::size_t>(size), 0));
}

ESC::escSize_t OSAL::OSALAdapter::adapterWrite(ESC::EthercatFrame& frame)
{
    return adapterWrite(frame.pData(), frame.size());
}

ESC::escSize_t OSAL::OSALAdapter::adapterRead(ESC::EthercatFrame& frame)
{
    ESC::escSize_t rxSize = 0;
 
    rxSize = adapterRead(frame.pData(), static_cast<std::size_t>(frame.maxEthFrameSize()));
    
    if ((rxSize == 0xFFFF) || (rxSize == 0))
    {
        //fprintf(stderr, "socket() failed: %s\n", strerror(errno));
    }
    else
    {
        frame.setFrame(rxSize);
    }
    return rxSize;
}

