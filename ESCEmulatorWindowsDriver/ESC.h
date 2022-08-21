#pragma once
#include <ntdef.h>

//Marcel Siegwart extend OID with ESC Funktionality
#define OID_ESC_PARAM_SIZE          2048
#define OID_ESC_CMD                 0x0F000000

typedef struct _OID_ESC_CMD_STRUCT
{
    ULONG           Length;             // Length of structure
    ULONG           CMD;
    UCHAR           DATA[OID_ESC_PARAM_SIZE];
}OID_ESC_CMD_STRUCT, * POID_ESC_CMD_STRUCT;

LONG ESCAddEsc(ULONG PID);
LONG ESCRemoveEsc(ULONG PID);
LONG ESCGetEsc(ULONG Number);
LONG ESCSetRegisterAddress(USHORT Address);
LONG ESCWriteRegister(USHORT Len, PUSHORT Data);
LONG ESCReadRegister(USHORT Len, PUSHORT Data);
LONG ESCProcessFrame(PUCHAR Frame, ULONG Lenght);
