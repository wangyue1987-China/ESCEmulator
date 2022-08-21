#include "ESC.h"
LONG ESCAddEsc(ULONG PID)
{
	UNREFERENCED_PARAMETER(PID);
	return -1;
}
LONG ESCRemoveEsc(ULONG PID)
{
	UNREFERENCED_PARAMETER(PID);
	return -1;
}
LONG ESCGetEsc(ULONG Number)
{
	UNREFERENCED_PARAMETER(Number);
	return -1;
}
LONG ESCSetRegisterAddress(USHORT Address)
{
	UNREFERENCED_PARAMETER(Address);
	return -1;
}
LONG ESCWriteRegister(USHORT Len, PUSHORT Data)
{
	UNREFERENCED_PARAMETER(Len);
	UNREFERENCED_PARAMETER(Data);
	return -1;
}
LONG ESCReadRegister(USHORT Len, PUSHORT Data)
{
	UNREFERENCED_PARAMETER(Len);
	UNREFERENCED_PARAMETER(Data);
	return -1;
}

LONG ESCProcessFrame(PUCHAR Frame, ULONG Lenght)
{
	UNREFERENCED_PARAMETER(Lenght);
	UCHAR tmp[6];
	for(INT i = 0; i < 6;i++)
	{
		tmp[i] = Frame[i];
	}
	for (INT i = 0; i < 6; i++)
	{
		Frame[i] = Frame[i + 6];
	}
	for (INT i = 0; i < 6; i++)
	{
		Frame[i + 6] = tmp[i];
	}
	return -1;
}
