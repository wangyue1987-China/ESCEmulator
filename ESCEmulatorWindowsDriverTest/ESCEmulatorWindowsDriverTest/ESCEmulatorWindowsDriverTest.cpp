// ESCEmulatorWindowsDriverTest.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <stdio.h>
#include <conio.h>
#define PCAP_DONT_INCLUDE_PCAP_BPF_H
#include <Packet32.h>
#include <ntddndis.h>
#include "pcap.h"

#define Max_Num_Adapter 10
char		AdapterList[Max_Num_Adapter][1024];


#include <tchar.h>
BOOL LoadNpcapDlls()
{
    _TCHAR npcap_dir[512];
    UINT len;
    len = GetSystemDirectory(npcap_dir, 480);
    if (!len) {
        fprintf(stderr, "Error in GetSystemDirectory: %x", GetLastError());
        return FALSE;
    }
    _tcscat_s(npcap_dir, 512, _T("\\Npcap"));
    if (SetDllDirectory(npcap_dir) == 0) {
        fprintf(stderr, "Error in SetDllDirectory: %x", GetLastError());
        return FALSE;
    }
    return TRUE;
}


int main()
{
	LPADAPTER	lpAdapter = 0;
	int			i;
	DWORD		dwErrorCode;
	char		AdapterName[8192];
	char* temp, * temp1;
	int			AdapterNum = 0, Open;
	ULONG		AdapterLength;
	PPACKET_OID_DATA  OidData;
	BOOLEAN		Status;

	/* Load Npcap and its functions. */
	if (!LoadNpcapDlls())
	{
		fprintf(stderr, "Couldn't load Npcap\n");
		exit(1);
	}

	//
	// Obtain the name of the adapters installed on this machine
	//

	printf("Packet.dll test application. Library version:%s\n", PacketGetVersion());

	printf("Adapters installed:\n");
	i = 0;

	AdapterLength = sizeof(AdapterName);

	if (PacketGetAdapterNames(AdapterName, &AdapterLength) == FALSE) {
		printf("Unable to retrieve the list of the adapters!\n");
		return -1;
	}
	temp = AdapterName;
	temp1 = AdapterName;

	while ((*temp != '\0') || (*(temp - 1) != '\0'))
	{
		if (*temp == '\0')
		{
			memcpy(AdapterList[i], temp1, temp - temp1);
			temp1 = temp + 1;
			i++;
		}
		temp++;
	}

	AdapterNum = i;
	for (i = 0; i < AdapterNum; i++)
		
		printf("\n%d- %s\n", i + 1, AdapterList[i]);
	printf("\n");

	pcap_if_t* alldevs;
	pcap_if_t* d;
	char errbuf[PCAP_ERRBUF_SIZE];

	/* Retrieve the device list from the local machine */
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL /* auth is not needed */, &alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", errbuf);
		exit(1);
	}

	/* Print the list */
	for (d = alldevs; d != NULL; d = d->next)
	{
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}

	if (i == 0)
	{
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return 0;
	}

	/* We don't need any more the device list. Free it */
	pcap_freealldevs(alldevs);

	do
	{
		printf("Select the number of the adapter to open : ");
		scanf_s("%d", &Open);
		if (Open > AdapterNum) printf("\nThe number must be smaller than %d", AdapterNum);
	} while (Open > AdapterNum);


	//
	// Open the selected adapter
	//

	lpAdapter = PacketOpenAdapter(AdapterList[Open - 1]);

	if (!lpAdapter || (lpAdapter->hFile == INVALID_HANDLE_VALUE))
	{
		dwErrorCode = GetLastError();
		printf("Unable to open the adapter, Error Code : %lx\n", dwErrorCode);

		return -1;
	}

	// 
	// Allocate a buffer to get the MAC adress
	//

	OidData = (PPACKET_OID_DATA)malloc(6 + sizeof(PACKET_OID_DATA));
	if (OidData == NULL)
	{
		printf("error allocating memory!\n");
		PacketCloseAdapter(lpAdapter);
		return -1;
	}

	// 
	// Retrieve the adapter MAC querying the NIC driver
	//

	OidData->Oid = 0xF0000000;
	(OidData->Data)[0] = 0x11;
	(OidData->Data)[1] = 0x12;
	(OidData->Data)[2] = 0x13;
	(OidData->Data)[3] = 0x14;
	(OidData->Data)[4] = 0x15;
	(OidData->Data)[5] = 0x16;
	OidData->Length = 6;
	//ZeroMemory(OidData->Data, 6);

	Status = PacketRequest(lpAdapter, TRUE, OidData);
	if (Status)
	{
		printf("The MAC address of the adapter is %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
			(OidData->Data)[0],
			(OidData->Data)[1],
			(OidData->Data)[2],
			(OidData->Data)[3],
			(OidData->Data)[4],
			(OidData->Data)[5]);
	}
	else
	{
		printf("error retrieving the MAC address of the adapter!\n");
	}

	free(OidData);
	PacketCloseAdapter(lpAdapter);
	return (0);
}

// Programm ausführen: STRG+F5 oder Menüeintrag "Debuggen" > "Starten ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
