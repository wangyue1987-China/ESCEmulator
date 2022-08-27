/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

Module Name:

   Miniport.C

Abstract:

    The purpose of this sample is to illustrate functionality of a deserialized
    NDIS miniport driver without requiring a physical network adapter. This
    sample is based on E100BEX sample present in the DDK. It is basically a
    simplified version of E100bex driver. The driver can be installed either
    manually using Add Hardware wizard as a root enumerated virtual miniport
    driver or on a virtual bus (like toaster bus). Since the driver does not
    interact with any hardware, it makes it very easy to understand the miniport
    interface and the usage of various NDIS functions without the clutter of
    hardware specific code normally found in a fully functional driver.

    This sample provides an example of minimal driver intended for education
    purposes. Neither the driver or its sample test programs are intended
    for use in a production environment.

Revision History:

Notes:

--*/
#include "netvmin6.h"
#include "miniport.tmh"
#include "miniport.h"




NDIS_STATUS
DriverEntry(
    _In_  PVOID DriverObject,
    _In_  PVOID RegistryPath);

_Dispatch_type_(IRP_MJ_CREATE) DRIVER_DISPATCH EscMiniPortOpen;
#if 0
NTSTATUS
EscMiniPortOpen(
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
);

_Dispatch_type_(IRP_MJ_CLOSE) DRIVER_DISPATCH EscMiniPortClose;
NTSTATUS
EscMiniPortClose(
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
);

_Dispatch_type_(IRP_MJ_CLEANUP) DRIVER_DISPATCH EscMiniPortCleanup;
NTSTATUS
EscMiniPortCleanup(
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
);

_Dispatch_type_(IRP_MJ_DEVICE_CONTROL) DRIVER_DISPATCH EscMiniPortIoControl;
//_Function_class_(DRIVER_DISPATCH)
//_IRQL_requires_(PASSIVE_LEVEL)
//_IRQL_requires_same_
NTSTATUS
EscMiniPortIoControl(
    _In_ IN PDEVICE_OBJECT   pDeviceObject,
    _Inout_  IN PIRP             pIrp
);


_Dispatch_type_(IRP_MJ_READ) DRIVER_DISPATCH NdisprotRead;
NTSTATUS
EscMiniPortRead(
    IN PDEVICE_OBJECT               pDeviceObject,
    IN PIRP                         pIrp
);

DRIVER_CANCEL NdisprotCancelRead;
VOID
EscMiniPortCancelRead(
    IN PDEVICE_OBJECT               pDeviceObject,
    IN PIRP                         pIrp
);

VOID
EscMiniPortServiceReads(
    IN PVOID        pOpenContext
);

VOID
EscMiniPortWrite(
    IN PDEVICE_OBJECT       pDeviceObject,
    IN PIRP                 pIrp
);

DRIVER_CANCEL NdisprotCancelWrite;
VOID
EscMiniPortCancelWrite(
    IN PDEVICE_OBJECT               pDeviceObject,
    IN PIRP                         pIrp
);
#endif
#pragma NDIS_INIT_FUNCTION(DriverEntry)
#pragma NDIS_PAGEABLE_FUNCTION(DriverUnload)


MP_GLOBAL       GlobalData;
NDIS_HANDLE     NdisDriverHandle;

static
NDIS_STATUS
InitializeAdapterListLock();

static
VOID
FreeAdapterListLock();

NDIS_STATUS
DriverEntry(
    _In_  PVOID DriverObject,
    _In_  PVOID RegistryPath)
/*++
Routine Description:

    In the context of its DriverEntry function, a miniport driver associates
    itself with NDIS, specifies the NDIS version that it is using, and
    registers its entry points.


Arguments:
    PVOID DriverObject - pointer to the driver object.
    PVOID RegistryPath - pointer to the driver registry path.

    Return Value:

    NTSTATUS code

--*/
{
    NDIS_STATUS Status;
    NDIS_MINIPORT_DRIVER_CHARACTERISTICS MPChar;


    WPP_INIT_TRACING(DriverObject,RegistryPath);

    DEBUGP(MP_TRACE, "---> DriverEntry built on "__DATE__" at "__TIME__ "\n");
    PAGED_CODE();

    do
    {
        //
        // Initialize any driver-global variables here.
        //

        NdisZeroMemory(&GlobalData, sizeof(GlobalData));

        //
        // The ApaterList in the GlobalData structure is used to track multiple
        // adapters controlled by this miniport.
        //
        NdisInitializeListHead(&GlobalData.AdapterList);


        //
        // The FrameDataLookaside list is used to help emulate an Ethernet hub.
        //
        NdisInitializeNPagedLookasideList(
                &GlobalData.FrameDataLookaside,
                HWFrameAllocate,
                HWFrameFree,
                0,  // Reserved for system use
                sizeof(FRAME),
                NIC_TAG_FRAME,
                0); // Reserved for system use
        GlobalData.Flags |= fGLOBAL_LOOKASIDE_INITIALIZED;

        //
        // Fill in the Miniport characteristics structure with the version numbers
        // and the entry points for driver-supplied MiniportXxx
        //

        NdisZeroMemory(&MPChar, sizeof(MPChar));


#if (NDIS_SUPPORT_NDIS620)
        {C_ASSERT(sizeof(MPChar) >= NDIS_SIZEOF_MINIPORT_DRIVER_CHARACTERISTICS_REVISION_2);}
        MPChar.Header.Type = NDIS_OBJECT_TYPE_MINIPORT_DRIVER_CHARACTERISTICS;
        MPChar.Header.Size = NDIS_SIZEOF_MINIPORT_DRIVER_CHARACTERISTICS_REVISION_2;
        MPChar.Header.Revision = NDIS_MINIPORT_DRIVER_CHARACTERISTICS_REVISION_2;
#elif (NDIS_SUPPORT_NDIS6)
        {C_ASSERT(sizeof(MPChar) >= NDIS_SIZEOF_MINIPORT_DRIVER_CHARACTERISTICS_REVISION_1);}
        MPChar.Header.Type = NDIS_OBJECT_TYPE_MINIPORT_DRIVER_CHARACTERISTICS;
        MPChar.Header.Size = NDIS_SIZEOF_MINIPORT_DRIVER_CHARACTERISTICS_REVISION_1;
        MPChar.Header.Revision = NDIS_MINIPORT_DRIVER_CHARACTERISTICS_REVISION_1;
#endif // NDIS MINIPORT VERSION

        MPChar.MajorNdisVersion = MP_NDIS_MAJOR_VERSION;
        MPChar.MinorNdisVersion = MP_NDIS_MINOR_VERSION;

        MPChar.MajorDriverVersion = NIC_MAJOR_DRIVER_VERSION;
        MPChar.MinorDriverVersion = NIC_MINOR_DRIVER_VERISON;

        MPChar.Flags = 0;

        MPChar.SetOptionsHandler = MPSetOptions; // Optional
        MPChar.InitializeHandlerEx = MPInitializeEx;
        MPChar.HaltHandlerEx = MPHaltEx;
        MPChar.UnloadHandler = DriverUnload;
        MPChar.PauseHandler = MPPause;
        MPChar.RestartHandler = MPRestart;
        MPChar.OidRequestHandler = MPOidRequest;
        MPChar.SendNetBufferListsHandler = MPSendNetBufferLists;
        MPChar.ReturnNetBufferListsHandler = MPReturnNetBufferLists;
        MPChar.CancelSendHandler = MPCancelSend;
        MPChar.CheckForHangHandlerEx = MPCheckForHangEx;
        MPChar.ResetHandlerEx = MPResetEx;
        MPChar.DevicePnPEventNotifyHandler = MPDevicePnpEventNotify;
        MPChar.ShutdownHandlerEx = MPShutdownEx;
        MPChar.CancelOidRequestHandler = MPCancelOidRequest;
       
      

    
#if (NDIS_SUPPORT_NDIS680)
        MPChar.SynchronousOidRequestHandler = MPSynchronousOidRequest;
#endif

        //
        // Associate the miniport driver with NDIS by calling the
        // NdisMRegisterMiniportDriver. This function returns an NdisDriverHandle.
        // The miniport driver must retain this handle but it should never attempt
        // to access or interpret this handle.
        //
        // By calling NdisMRegisterMiniportDriver, the driver indicates that it
        // is ready for NDIS to call the driver's MiniportSetOptions and
        // MiniportInitializeEx handlers.
        //
        DEBUGP(MP_LOUD, "Calling NdisMRegisterMiniportDriver...\n");
        NDIS_DECLARE_MINIPORT_DRIVER_CONTEXT(MP_GLOBAL);
        Status = NdisMRegisterMiniportDriver(
                DriverObject,
                RegistryPath,
                &GlobalData,
                &MPChar,
                &NdisDriverHandle);
        if (NDIS_STATUS_SUCCESS != Status)
        {
            DEBUGP(MP_ERROR, "NdisMRegisterMiniportDriver failed: %d\n", Status);
            DriverUnload(DriverObject);
            Status = NDIS_STATUS_FAILURE;
            break;
        }

        GlobalData.Flags |= fGLOBAL_MINIPORT_REGISTERED;

        //
        // The AdapterListLock protects the AdapterList
        //
        Status = InitializeAdapterListLock();
        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(MP_ERROR, "InitializeAdapterListLock failed 0x%08x", Status);
            DriverUnload(DriverObject);
            Status = NDIS_STATUS_FAILURE;
            break;
        }
        GlobalData.Flags |= fGLOBAL_LOCK_ALLOCATED;
    }while(FALSE);

    DEBUGP(MP_TRACE, "<--- DriverEntry Status 0x%08x\n", Status);
    return Status;
}


VOID
DriverUnload(
    _In_  PDRIVER_OBJECT  DriverObject)
/*++

Routine Description:

    The unload handler is called during driver unload to free up resources
    acquired in DriverEntry. This handler is registered in DriverEntry through
    NdisMRegisterMiniportDriver. Note that an unload handler differs from
    a MiniportHalt function in that this unload handler releases resources that
    are global to the driver, while the halt handler releases resource for a
    particular adapter.

    Runs at IRQL = PASSIVE_LEVEL.

Arguments:

    DriverObject        Not used

Return Value:

    None.

--*/
{
    UNREFERENCED_PARAMETER(DriverObject);

    DEBUGP(MP_TRACE, "---> DriverUnload\n");
    PAGED_CODE();


    //
    // Clean up all globals that were allocated in DriverEntry
    //


    ASSERT(IsListEmpty(&GlobalData.AdapterList));


    if (GlobalData.Flags & fGLOBAL_MINIPORT_REGISTERED)
    {
        //
        // Since DriverEntry has successfully called NdisMRegisterMiniportDriver,
        // NdisMDeregisterMiniportDriver must be called to release NDIS's per-driver
        // resources.
        //
        DEBUGP(MP_LOUD, "Calling NdisMDeregisterMiniportDriver...\n");
        NdisMDeregisterMiniportDriver(NdisDriverHandle);
    }

    if (GlobalData.Flags & fGLOBAL_LOOKASIDE_INITIALIZED)
    {
        NdisDeleteNPagedLookasideList(&GlobalData.FrameDataLookaside);
    }

    if (GlobalData.Flags & fGLOBAL_LOCK_ALLOCATED)
    {
        FreeAdapterListLock();
    }

    WPP_CLEANUP(DriverObject->DeviceObject);

    DEBUGP(MP_TRACE, "<--- DriverUnload\n");
}


NDIS_STATUS
MPSetOptions(
    _In_  NDIS_HANDLE  DriverHandle,
    _In_  NDIS_HANDLE  DriverContext)
/*++
Routine Description:

    The MiniportSetOptions function registers optional handlers.  For each
    optional handler that should be registered, this function makes a call
    to NdisSetOptionalHandlers.

    MiniportSetOptions runs at IRQL = PASSIVE_LEVEL.

Arguments:

    DriverContext  The context handle

Return Value:

    NDIS_STATUS_xxx code

--*/
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
    PMP_GLOBAL Global = (PMP_GLOBAL)DriverContext;

    DEBUGP(MP_TRACE, "---> MPSetOptions\n");
    UNREFERENCED_PARAMETER(DriverHandle);
    UNREFERENCED_PARAMETER(Global);


    //
    // Set any optional handlers by filling out the appropriate struct and
    // calling NdisSetOptionalHandlers here.
    //


    DEBUGP(MP_TRACE, "<--- MPSetOptions Status = 0x%08x\n", Status);
    return Status;
}

BOOLEAN
MPIsAdapterAttached(
    _In_ PMP_ADAPTER Adapter)
{
    PLIST_ENTRY CurrentEntry = NULL;
    for (
            CurrentEntry = GlobalData.AdapterList.Flink;
            CurrentEntry != &GlobalData.AdapterList;
            CurrentEntry = CurrentEntry->Flink)
    {
        if(CurrentEntry == &Adapter->List)
        {
            return TRUE;
        }
    }

    return FALSE;
}

VOID
MPAttachAdapter(
    _In_  PMP_ADAPTER Adapter)
{
    MP_LOCK_STATE LockState;    

    DEBUGP(MP_TRACE, "[%p] ---> MPAttachAdapter\n", Adapter);

    LOCK_ADAPTER_LIST_FOR_WRITE(&LockState, 0);

    if(!MPIsAdapterAttached(Adapter))
    {
        InsertTailList(&GlobalData.AdapterList, &Adapter->List);
    }

    UNLOCK_ADAPTER_LIST(&LockState);

    DEBUGP(MP_TRACE, "[%p] <--- MPAttachAdapter\n", Adapter);
}

VOID
MPDetachAdapter(
    _In_  PMP_ADAPTER Adapter)
{
    MP_LOCK_STATE LockState;
    DEBUGP(MP_TRACE, "[%p] ---> MPDetachAdapter\n", Adapter);

    LOCK_ADAPTER_LIST_FOR_WRITE(&LockState, 0);

    if(MPIsAdapterAttached(Adapter))
    {
        RemoveEntryList(&Adapter->List);
    }

    UNLOCK_ADAPTER_LIST(&LockState);

    DEBUGP(MP_TRACE, "[%p] <--- MPDetachAdapter\n", Adapter);
}


#if (NDIS_SUPPORT_NDIS620)

NDIS_STATUS
InitializeAdapterListLock()
{
    GlobalData.Lock = NdisAllocateRWLock(NdisDriverHandle);
    if (!GlobalData.Lock)
    {
        return NDIS_STATUS_RESOURCES;
    }

    return NDIS_STATUS_SUCCESS;
}

VOID
FreeAdapterListLock()
{
    ASSERT(GlobalData.Lock);
    NdisFreeRWLock(GlobalData.Lock);
    GlobalData.Lock = NULL;
}

#elif (NDIS_SUPPORT_NDIS6)

NDIS_STATUS
InitializeAdapterListLock()
{
    NdisInitializeReadWriteLock(&GlobalData.Lock);
    return NDIS_STATUS_SUCCESS;
}

VOID
FreeAdapterListLock()
{
    // No action needed to clean up a NDIS 6.0 RW lock.
}

#endif



#ifndef DBG

VOID
DbgPrintOidName(
    _In_  NDIS_OID  Oid)
{
    UNREFERENCED_PARAMETER(Oid);
}

VOID
DbgPrintAddress(
    _In_reads_bytes_(NIC_MACADDR_SIZE) PUCHAR Address)
{
    UNREFERENCED_PARAMETER(Address);
}


#else // DBG

VOID
DbgPrintOidName(
    _In_  NDIS_OID  Oid)
{
    PCHAR oidName = NULL;

    switch (Oid){

        #undef MAKECASE
        #define MAKECASE(oidx) case oidx: oidName = #oidx "\n"; break;

        /* Operational OIDs */
        MAKECASE(OID_GEN_SUPPORTED_LIST)
        MAKECASE(OID_GEN_HARDWARE_STATUS)
        MAKECASE(OID_GEN_MEDIA_SUPPORTED)
        MAKECASE(OID_GEN_MEDIA_IN_USE)
        MAKECASE(OID_GEN_MAXIMUM_LOOKAHEAD)
        MAKECASE(OID_GEN_MAXIMUM_FRAME_SIZE)
        MAKECASE(OID_GEN_LINK_SPEED)
        MAKECASE(OID_GEN_TRANSMIT_BUFFER_SPACE)
        MAKECASE(OID_GEN_RECEIVE_BUFFER_SPACE)
        MAKECASE(OID_GEN_TRANSMIT_BLOCK_SIZE)
        MAKECASE(OID_GEN_RECEIVE_BLOCK_SIZE)
        MAKECASE(OID_GEN_VENDOR_ID)
        MAKECASE(OID_GEN_VENDOR_DESCRIPTION)
        MAKECASE(OID_GEN_VENDOR_DRIVER_VERSION)
        MAKECASE(OID_GEN_CURRENT_PACKET_FILTER)
        MAKECASE(OID_GEN_CURRENT_LOOKAHEAD)
        MAKECASE(OID_GEN_DRIVER_VERSION)
        MAKECASE(OID_GEN_MAXIMUM_TOTAL_SIZE)
        MAKECASE(OID_GEN_PROTOCOL_OPTIONS)
        MAKECASE(OID_GEN_MAC_OPTIONS)
        MAKECASE(OID_GEN_MEDIA_CONNECT_STATUS)
        MAKECASE(OID_GEN_MAXIMUM_SEND_PACKETS)
        MAKECASE(OID_GEN_SUPPORTED_GUIDS)
        MAKECASE(OID_GEN_NETWORK_LAYER_ADDRESSES)
        MAKECASE(OID_GEN_TRANSPORT_HEADER_OFFSET)
        MAKECASE(OID_GEN_MEDIA_CAPABILITIES)
        MAKECASE(OID_GEN_PHYSICAL_MEDIUM)
        MAKECASE(OID_GEN_MACHINE_NAME)
        MAKECASE(OID_GEN_VLAN_ID)
        MAKECASE(OID_GEN_RNDIS_CONFIG_PARAMETER)

        /* Operational OIDs for NDIS 6.0 */
        MAKECASE(OID_GEN_MAX_LINK_SPEED)
        MAKECASE(OID_GEN_LINK_STATE)
        MAKECASE(OID_GEN_LINK_PARAMETERS)
        MAKECASE(OID_GEN_MINIPORT_RESTART_ATTRIBUTES)
        MAKECASE(OID_GEN_ENUMERATE_PORTS)
        MAKECASE(OID_GEN_PORT_STATE)
        MAKECASE(OID_GEN_PORT_AUTHENTICATION_PARAMETERS)
        MAKECASE(OID_GEN_INTERRUPT_MODERATION)
        MAKECASE(OID_GEN_PHYSICAL_MEDIUM_EX)

        /* Statistical OIDs */
        MAKECASE(OID_GEN_XMIT_OK)
        MAKECASE(OID_GEN_RCV_OK)
        MAKECASE(OID_GEN_XMIT_ERROR)
        MAKECASE(OID_GEN_RCV_ERROR)
        MAKECASE(OID_GEN_RCV_NO_BUFFER)
        MAKECASE(OID_GEN_DIRECTED_BYTES_XMIT)
        MAKECASE(OID_GEN_DIRECTED_FRAMES_XMIT)
        MAKECASE(OID_GEN_MULTICAST_BYTES_XMIT)
        MAKECASE(OID_GEN_MULTICAST_FRAMES_XMIT)
        MAKECASE(OID_GEN_BROADCAST_BYTES_XMIT)
        MAKECASE(OID_GEN_BROADCAST_FRAMES_XMIT)
        MAKECASE(OID_GEN_DIRECTED_BYTES_RCV)
        MAKECASE(OID_GEN_DIRECTED_FRAMES_RCV)
        MAKECASE(OID_GEN_MULTICAST_BYTES_RCV)
        MAKECASE(OID_GEN_MULTICAST_FRAMES_RCV)
        MAKECASE(OID_GEN_BROADCAST_BYTES_RCV)
        MAKECASE(OID_GEN_BROADCAST_FRAMES_RCV)
        MAKECASE(OID_GEN_RCV_CRC_ERROR)
        MAKECASE(OID_GEN_TRANSMIT_QUEUE_LENGTH)

        /* Statistical OIDs for NDIS 6.0 */
        MAKECASE(OID_GEN_STATISTICS)
        MAKECASE(OID_GEN_BYTES_RCV)
        MAKECASE(OID_GEN_BYTES_XMIT)
        MAKECASE(OID_GEN_RCV_DISCARDS)
        MAKECASE(OID_GEN_XMIT_DISCARDS)

        /* Misc OIDs */
        MAKECASE(OID_GEN_GET_TIME_CAPS)
        MAKECASE(OID_GEN_GET_NETCARD_TIME)
        MAKECASE(OID_GEN_NETCARD_LOAD)
        MAKECASE(OID_GEN_DEVICE_PROFILE)
        MAKECASE(OID_GEN_INIT_TIME_MS)
        MAKECASE(OID_GEN_RESET_COUNTS)
        MAKECASE(OID_GEN_MEDIA_SENSE_COUNTS)

        /* PnP power management operational OIDs */
        MAKECASE(OID_PNP_CAPABILITIES)
        MAKECASE(OID_PNP_SET_POWER)
        MAKECASE(OID_PNP_QUERY_POWER)
        MAKECASE(OID_PNP_ADD_WAKE_UP_PATTERN)
        MAKECASE(OID_PNP_REMOVE_WAKE_UP_PATTERN)
        MAKECASE(OID_PNP_ENABLE_WAKE_UP)
        MAKECASE(OID_PNP_WAKE_UP_PATTERN_LIST)

        /* PnP power management statistical OIDs */
        MAKECASE(OID_PNP_WAKE_UP_ERROR)
        MAKECASE(OID_PNP_WAKE_UP_OK)

        /* Ethernet operational OIDs */
        MAKECASE(OID_802_3_PERMANENT_ADDRESS)
        MAKECASE(OID_802_3_CURRENT_ADDRESS)
        MAKECASE(OID_802_3_MULTICAST_LIST)
        MAKECASE(OID_802_3_MAXIMUM_LIST_SIZE)
        MAKECASE(OID_802_3_MAC_OPTIONS)

        /* Ethernet operational OIDs for NDIS 6.0 */
        MAKECASE(OID_802_3_ADD_MULTICAST_ADDRESS)
        MAKECASE(OID_802_3_DELETE_MULTICAST_ADDRESS)

        /* Ethernet statistical OIDs */
        MAKECASE(OID_802_3_RCV_ERROR_ALIGNMENT)
        MAKECASE(OID_802_3_XMIT_ONE_COLLISION)
        MAKECASE(OID_802_3_XMIT_MORE_COLLISIONS)
        MAKECASE(OID_802_3_XMIT_DEFERRED)
        MAKECASE(OID_802_3_XMIT_MAX_COLLISIONS)
        MAKECASE(OID_802_3_RCV_OVERRUN)
        MAKECASE(OID_802_3_XMIT_UNDERRUN)
        MAKECASE(OID_802_3_XMIT_HEARTBEAT_FAILURE)
        MAKECASE(OID_802_3_XMIT_TIMES_CRS_LOST)
        MAKECASE(OID_802_3_XMIT_LATE_COLLISIONS)

        /*  TCP/IP OIDs */
        MAKECASE(OID_TCP_TASK_OFFLOAD)
        MAKECASE(OID_TCP_TASK_IPSEC_ADD_SA)
        MAKECASE(OID_TCP_TASK_IPSEC_DELETE_SA)
        MAKECASE(OID_TCP_SAN_SUPPORT)
        MAKECASE(OID_TCP_TASK_IPSEC_ADD_UDPESP_SA)
        MAKECASE(OID_TCP_TASK_IPSEC_DELETE_UDPESP_SA)
        MAKECASE(OID_TCP4_OFFLOAD_STATS)
        MAKECASE(OID_TCP6_OFFLOAD_STATS)
        MAKECASE(OID_IP4_OFFLOAD_STATS)
        MAKECASE(OID_IP6_OFFLOAD_STATS)

        /* TCP offload OIDs for NDIS 6 */
        MAKECASE(OID_TCP_OFFLOAD_CURRENT_CONFIG)
        MAKECASE(OID_TCP_OFFLOAD_PARAMETERS)
        MAKECASE(OID_TCP_OFFLOAD_HARDWARE_CAPABILITIES)
        MAKECASE(OID_TCP_CONNECTION_OFFLOAD_CURRENT_CONFIG)
        MAKECASE(OID_TCP_CONNECTION_OFFLOAD_HARDWARE_CAPABILITIES)
        MAKECASE(OID_OFFLOAD_ENCAPSULATION)
        MAKECASE(OID_OEM_ESC_GET)
        MAKECASE(OID_OEM_ESC_SET)

#if (NDIS_SUPPORT_NDIS620)
        /* VMQ OIDs for NDIS 6.20 */
        MAKECASE(OID_RECEIVE_FILTER_FREE_QUEUE)
        MAKECASE(OID_RECEIVE_FILTER_CLEAR_FILTER)
        MAKECASE(OID_RECEIVE_FILTER_ALLOCATE_QUEUE)
        MAKECASE(OID_RECEIVE_FILTER_QUEUE_ALLOCATION_COMPLETE)
        MAKECASE(OID_RECEIVE_FILTER_SET_FILTER)
#endif

#if (NDIS_SUPPORT_NDIS630)
        /* NDIS QoS OIDs for NDIS 6.30 */
        MAKECASE(OID_QOS_PARAMETERS)
#endif

#if (NDIS_SUPPORT_NDIS680)
        /* RSSv2 OIDS*/
        MAKECASE(OID_GEN_RSS_SET_INDIRECTION_TABLE_ENTRIES)
        MAKECASE(OID_GEN_RECEIVE_SCALE_PARAMETERS_V2)
#endif
    }

    if (oidName)
    {
        DEBUGP(MP_LOUD, "%s", oidName);
    }
    else
    {
        DEBUGP(MP_LOUD, "<** Unknown OID 0x%08x **>\n", Oid);
    }
}

VOID
DbgPrintAddress(
    _In_reads_bytes_(NIC_MACADDR_SIZE) PUCHAR Address)
{
    // If your MAC address has a different size, adjust the printf accordingly.
    {C_ASSERT(NIC_MACADDR_SIZE == 6);}

    DEBUGP(MP_LOUD, "%02x-%02x-%02x-%02x-%02x-%02x\n",
            Address[0], Address[1], Address[2],
            Address[3], Address[4], Address[5]);
}

#if 0
NTSTATUS
EscMiniPortOpen(
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
)
/*++

Routine Description:

    This is the dispatch routine for handling IRP_MJ_CREATE.
    We simply succeed this.

Arguments:

    pDeviceObject - Pointer to the device object.

    pIrp - Pointer to the request packet.

Return Value:

    Status is returned.

--*/
{
    PIO_STACK_LOCATION      pIrpSp;
    NTSTATUS                NtStatus = STATUS_SUCCESS;
    PESCMINIPORT_OPEN_CONTEXT   pOpenContext;
    PAGED_CODE();
    UNREFERENCED_PARAMETER(pDeviceObject);

    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
    ASSERT(pIrpSp->FileObject != NULL);
    pOpenContext =ExAllocatePoolQuotaZero(NonPagedPool | POOL_QUOTA_FAIL_INSTEAD_OF_RAISE,
        sizeof(PESCMINIPORT_OPEN_CONTEXT),
        0x12345678);
    if (NULL == pOpenContext) {
        NtStatus = STATUS_INSUFFICIENT_RESOURCES;
       
    }
    pIrpSp->FileObject->FsContext = pOpenContext;
    DEBUGP(MP_TRACE, "Open: FileObject %p\n", pIrpSp->FileObject);

    pIrp->IoStatus.Information = 0;
    pIrp->IoStatus.Status = NtStatus;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    return NtStatus;
}

NTSTATUS
EscMiniPortClose(
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
)
/*++

Routine Description:

    This is the dispatch routine for handling IRP_MJ_CLOSE.
    We simply succeed this.

Arguments:

    pDeviceObject - Pointer to the device object.

    pIrp - Pointer to the request packet.

Return Value:

    Status is returned.

--*/
{
    NTSTATUS                NtStatus;
    PIO_STACK_LOCATION      pIrpSp;
    PESCMINIPORT_OPEN_CONTEXT   pOpenContext;
    PAGED_CODE();
    UNREFERENCED_PARAMETER(pDeviceObject);

    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
    pOpenContext = pIrpSp->FileObject->FsContext;

    DEBUGP(MP_LOUD, "Close: FileObject %p\n",
        IoGetCurrentIrpStackLocation(pIrp)->FileObject);


    pIrpSp->FileObject->FsContext = NULL;
    NtStatus = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0;
    pIrp->IoStatus.Status = NtStatus;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    return NtStatus;
}



NTSTATUS
EscMiniPortCleanup(
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
)
/*++

Routine Description:

    This is the dispatch routine for handling IRP_MJ_CLEANUP.

Arguments:

    pDeviceObject - Pointer to the device object.

    pIrp - Pointer to the request packet.

Return Value:

    Status is returned.

--*/
{
    PIO_STACK_LOCATION      pIrpSp;
    NTSTATUS                NtStatus;
   
    PVOID   pOpenContext;
  


    UNREFERENCED_PARAMETER(pDeviceObject);

    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
    pOpenContext = pIrpSp->FileObject->FsContext;

    DEBUGP(MP_LOUD, "Cleanup: FileObject %p, Open %p\n",
        pIrpSp->FileObject, pOpenContext);

    if (pOpenContext != NULL)
    {  
        pIrpSp->FileObject = NULL;
    }

    NtStatus = STATUS_SUCCESS;

    pIrp->IoStatus.Information = 0;
    pIrp->IoStatus.Status = NtStatus;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    DEBUGP(MP_LOUD, "Cleanup: OpenContext %p\n", pOpenContext);

    return (NtStatus);
}

NTSTATUS
EscMiniPortIoControl(
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
)
/*++

Routine Description:

    This is the dispatch routine for handling device ioctl requests.

Arguments:

    pDeviceObject - Pointer to the device object.

    pIrp - Pointer to the request packet.

Return Value:

    Status is returned.

--*/
{
    PIO_STACK_LOCATION      pIrpSp;
    ULONG                   FunctionCode;
    NTSTATUS                NtStatus;
   // PVOID  pOpenContext;
    ULONG                   BytesReturned;
#if !DBG
    UNREFERENCED_PARAMETER(pDeviceObject);
#endif
    PAGED_CODE();
    DEBUGP(MP_LOUD, "IoControl: DevObj %p, Irp %p\n", pDeviceObject, pIrp);

    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);

    FunctionCode = pIrpSp->Parameters.DeviceIoControl.IoControlCode;
    //pOpenContext = (PNDISPROT_OPEN_CONTEXT)pIrpSp->FileObject->FsContext;
    BytesReturned = 0;

    switch (FunctionCode)
    {
    case 1:
      

    default:

        NtStatus = STATUS_NOT_SUPPORTED;
        break;
    }

    if (NtStatus != STATUS_PENDING)
    {
        pIrp->IoStatus.Information = BytesReturned;
        pIrp->IoStatus.Status = NtStatus;
        IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    }

    return NtStatus;
}

VOID
EscMiniPortCancelRead(
    IN PDEVICE_OBJECT               pDeviceObject,
    IN PIRP                         pIrp
)
/*++

Routine Description:

    Cancel a pending read IRP. We unlink the IRP from the open context
    queue and complete it.

Arguments:

    pDeviceObject - pointer to our device object
    pIrp - IRP to be cancelled

Return Value:

    None

--*/
{
    PVOID       pOpenContext;

    UNREFERENCED_PARAMETER(pDeviceObject);

    IoReleaseCancelSpinLock(pIrp->CancelIrql);

    pOpenContext = (PVOID)pIrp->Tail.Overlay.DriverContext[0];
   

    DEBUGP(MP_LOUD, "CancelRead: Open %p, IRP %p\n", pOpenContext, pIrp);
    pIrp->IoStatus.Status = STATUS_CANCELLED;
    pIrp->IoStatus.Information = 0;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

   

}

NTSTATUS
EscMiniPortRead(
    IN PDEVICE_OBJECT       pDeviceObject,
    IN PIRP                 pIrp
)
/*++

Routine Description:

    Dispatch routine to handle IRP_MJ_READ.

Arguments:

    pDeviceObject - pointer to our device object
    pIrp - Pointer to request packet

Return Value:

    NT status code.

--*/
{
    PIO_STACK_LOCATION      pIrpSp;
    NTSTATUS                NtStatus;
    PVOID                   pOpenContext;

    UNREFERENCED_PARAMETER(pDeviceObject);

    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
   

    do
    {
        //
        // Validate!
        //
        if (pOpenContext == NULL)
        {
            DEBUGP(MP_LOUD, ("Read: NULL FsContext on FileObject %p\n",
                pIrpSp->FileObject));
            NtStatus = STATUS_INVALID_HANDLE;
            break;
        }

        if (pIrp->MdlAddress == NULL)
        {
            DEBUGP(MP_LOUD, ("Read: NULL MDL address on IRP %p\n", pIrp));
            NtStatus = STATUS_INVALID_PARAMETER;
            break;
        }

        //
        // Try to get a virtual address for the MDL.
        //
        if (MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, NormalPagePriority | MdlMappingNoExecute) == NULL)
        {
            DEBUGP(MP_LOUD, ("Read: MmGetSystemAddr failed for IRP %p, MDL %p\n",
                pIrp, pIrp->MdlAddress));
            NtStatus = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }
       

        IoSetCancelRoutine(pIrp, EscMiniPortCancelRead);

        if (pIrp->Cancel &&
            IoSetCancelRoutine(pIrp, NULL))
        {

            //
            // IRP has been canceled but the I/O manager did not manage to call our cancel routine. This
            // code is safe referencing the Irp->Cancel field without locks because of the memory barriers
            // in the interlocked exchange sequences used by IoSetCancelRoutine.
            //

            NtStatus = STATUS_CANCELLED;

            // IRP should be completed after releasing the lock

        }
        else
        {

           
            pIrp->Tail.Overlay.DriverContext[0] = (PVOID)pOpenContext;

            NPROT_REF_OPEN(pOpenContext);  // pended read IRP
           
            IoMarkIrpPending(pIrp);


            NtStatus = STATUS_PENDING;
        }

       

    } while (FALSE);

    if (NtStatus != STATUS_PENDING)
    {
        NPROT_ASSERT(NtStatus != STATUS_SUCCESS);
        pIrp->IoStatus.Information = 0;
        pIrp->IoStatus.Status = NtStatus;
        IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    }

    return (NtStatus);
}



VOID
EscMiniPortServiceReads(
    IN PVOID        pOpenContext
)
/*++

Routine Description:

    Utility routine to copy received data into user buffers and
    complete READ IRPs.

Arguments:

    pOpenContext - pointer to open context

Return Value:

    None

--*/
{
    PIRP                pIrp = NULL;
    PLIST_ENTRY         pIrpEntry;
    PNET_BUFFER_LIST    pRcvNetBufList;
    PLIST_ENTRY         pRcvNetBufListEntry;
    PUCHAR              pSrc, pDst;
    ULONG               BytesRemaining; // at pDst
    PMDL                pMdl;
    ULONG               BytesAvailable;
    BOOLEAN             FoundPendingIrp = FALSE;
    ULONG               SrcTotalLength = 0; // Source NetBuffer DataLenght
    ULONG               Offset = 0;         // CurrentMdlOffset
    ULONG               BytesToCopy = 0;

    DEBUGP(MP_LOUD, ("ServiceReads: open %p/%x\n",
        pOpenContext, pOpenContext->Flags));

    NPROT_REF_OPEN(pOpenContext);  // temp ref - service reads

    NPROT_ACQUIRE_LOCK(&pOpenContext->Lock, FALSE);

    while (!NPROT_IS_LIST_EMPTY(&pOpenContext->PendedReads) &&
        !NPROT_IS_LIST_EMPTY(&pOpenContext->RecvNetBufListQueue))
    {
        FoundPendingIrp = FALSE;

        //
        //  Get the first pended Read IRP
        //
        pIrpEntry = pOpenContext->PendedReads.Flink;
        while (pIrpEntry != &pOpenContext->PendedReads)
        {
            pIrp = CONTAINING_RECORD(pIrpEntry, IRP, Tail.Overlay.ListEntry);

            //
            //  Check to see if it is being cancelled.
            //
            if (IoSetCancelRoutine(pIrp, NULL))
            {
                //
                //  It isn't being cancelled, and can't be cancelled henceforth.
                //
                NPROT_REMOVE_ENTRY_LIST(pIrpEntry);
                FoundPendingIrp = TRUE;
                break;

                //
                //  NOTE: we decrement PendedReadCount way below in the
                //  while loop, to avoid letting through a thread trying
                //  to unbind.
                //
            }
            else
            {
                //
                //  The IRP is being cancelled; let the cancel routine handle it.
                //
                DEBUGP(DL_INFO, ("ServiceReads: open %p, skipping cancelled IRP %p\n",
                    pOpenContext, pIrp));

                pIrpEntry = pIrpEntry->Flink;
            }
        }

        if (FoundPendingIrp == FALSE)
        {
            break;
        }
        //
        //  Get the first queued receive packet
        //
        pRcvNetBufListEntry = pOpenContext->RecvNetBufListQueue.Flink;
        NPROT_REMOVE_ENTRY_LIST(pRcvNetBufListEntry);

        pOpenContext->RecvNetBufListCount--;

        NPROT_RELEASE_LOCK(&pOpenContext->Lock, FALSE);

        NPROT_DEREF_OPEN(pOpenContext);  // Service: dequeue rcv packet

        pRcvNetBufList = NPROT_RCV_NBL_FROM_LIST_ENTRY(pRcvNetBufListEntry);
        NPROT_ASSERT(pRcvNetBufList != NULL);
        _Analysis_assume_(pRcvNetBufList != NULL);
        NPROT_RCV_NBL_FROM_LIST_ENTRY(pRcvNetBufListEntry) = NULL;

        //
        //  Copy as much data as possible from the receive packet to
        //  the IRP MDL.
        //

        pDst = NULL;
        NdisQueryMdl(pIrp->MdlAddress, &pDst, &BytesRemaining, NormalPagePriority | MdlMappingNoExecute);
        NPROT_ASSERT(pDst != NULL);  // since it was already mapped
        _Analysis_assume_(pDst != NULL);

        pMdl = NET_BUFFER_CURRENT_MDL(NET_BUFFER_LIST_FIRST_NB(pRcvNetBufList));

        //
        // Copy the data in the received packet into the buffer provided by the client.
        // If the length of the receive packet is greater than length of the given buffer,
        // we just copy as many bytes as we can. Once the buffer is full, we just discard
        // the rest of the data, and complete the IRP sucessfully even we only did a partial copy.
        //

        SrcTotalLength = NET_BUFFER_DATA_LENGTH(NET_BUFFER_LIST_FIRST_NB(pRcvNetBufList));
        Offset = NET_BUFFER_CURRENT_MDL_OFFSET(NET_BUFFER_LIST_FIRST_NB(pRcvNetBufList));

        while (BytesRemaining && (pMdl != NULL) && SrcTotalLength)
        {
            pSrc = NULL;
            NdisQueryMdl(pMdl, &pSrc, &BytesAvailable, NormalPagePriority | MdlMappingNoExecute);

            if (pSrc == NULL)
            {
                DEBUGP(DL_FATAL,
                    ("ServiceReads: Open %p, NdisQueryMdl failed for MDL %p\n",
                        pOpenContext, pMdl));
                break;
            }

            NPROT_ASSERT(BytesAvailable > Offset);

            BytesToCopy = MIN(BytesAvailable - Offset, BytesRemaining);
            BytesToCopy = MIN(BytesToCopy, SrcTotalLength);

            NPROT_COPY_MEM(pDst, pSrc + Offset, BytesToCopy);
            BytesRemaining -= BytesToCopy;
            pDst += BytesToCopy;
            SrcTotalLength -= BytesToCopy;

            //
            // CurrentMdlOffset is used only for the first Mdl processed. For the remaining Mdls, it is 0.
            //
            Offset = 0;

            NdisGetNextMdl(pMdl, &pMdl);
        }

        //
        //  Complete the IRP.
        //
        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = MmGetMdlByteCount(pIrp->MdlAddress) - BytesRemaining;

        DEBUGP(DL_INFO, ("ServiceReads: Open %p, IRP %p completed with %d bytes\n",
            pOpenContext, pIrp, (ULONG)pIrp->IoStatus.Information));

        IoCompleteRequest(pIrp, IO_NO_INCREMENT);

        ndisprotFreeReceiveNetBufferList(pOpenContext, pRcvNetBufList, FALSE);


        NPROT_DEREF_OPEN(pOpenContext);    // took out pended Read

        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock, FALSE);
        pOpenContext->PendedReadCount--;

    }

    NPROT_RELEASE_LOCK(&pOpenContext->Lock, FALSE);

    NPROT_DEREF_OPEN(pOpenContext);    // temp ref - service reads
}
#endif


#endif //DBG

