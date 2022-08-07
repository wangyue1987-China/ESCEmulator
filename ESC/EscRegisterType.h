#pragma once
#include <stdint.h>

typedef struct escFeature_t
{
	uint16_t FMMUOperation : 1;
	uint16_t UnusedRegisterAccess : 1;
	uint16_t DistributedClocks : 1;
	uint16_t DistributedClocks : 1;
	uint16_t LowJitterEBUS : 1;
	uint16_t EnhancedLinkDetectionEBUS : 1;
	uint16_t EnhancedLinkDetectionMII : 1;
	uint16_t SeparateHandlingOfFCSErrors : 1;
	uint16_t EnhancedDCSYNCActivation : 1;
	uint16_t EtherCATLRWcommandSupport : 1;
	uint16_t EtherCATReadWriteCommandSupport : 1;
	uint16_t FixedFMMUSyncManagerConfiguration : 1;
	uint16_t Reserved : 4;
}ESC_FEATURE;

#define ESC_ADR_ESCINFORMATION 0x0000
typedef struct escInformation_t
{
	uint8_t Type;
	uint8_t Revision;
	uint16_t Build;
	uint8_t FMMUCount;
	uint8_t SMCount;
	uint8_t RAMsize;
	uint8_t PortDesc;
	ESC_FEATURE ESCFeatureSupported;
};

#define ESC_ADR_STATION_ADDRESS 0x0010
typedef	uint16_t ConfiguredStationAddress_t;
#define ESC_ADR_STATION_ALIAS 0x0012
typedef uint16_t ConfiguredStationAlias_t;

#define ESC_ADR_REGISTER_WRITE_ENABLE 0x0020
typedef	uint8_t* RegisterWriteEnable_t;
#define ESC_ADR_REGISTER_WRITE_PROTECTION 0x0021
typedef uint8_t* RegisterWriteProtection_t;
#define ESC_ADR_ESC_WRITE_ENABLE 0x0030
typedef uint8_t* ESCwriteEnable_t;
#define ESC_ADR_ESC_WRITE_PROTECTION 0x0031
typedef uint8_t* ESCWriteProtection_t;

#define ESC_ADR_RESET_ECAT 0x0040
typedef uint8_t* ESCResetECAT_t;
#define ESC_ADR_RESET_PDI 0x0041
typedef uint8_t* ESCResetPDI_t;

#define ESC_ADR_ESC_DL_CONTROL 0x0100
typedef struct escDLControl_t
{
	uint32_t Forwardingrule : 1;
	uint32_t TemporaryUsOfSettings : 1;
	uint32_t Reserved1 : 6;
	uint32_t LoopPort0 : 2;
	uint32_t LoopPort1 : 2;
	uint32_t LoopPort2 : 2;
	uint32_t LoopPort3 : 2;
	uint32_t RXFIFOSize : 3;
	uint32_t EBUSLowJitter : 1;
	uint32_t Reserved2 : 2;
	uint32_t EBUSRemoteLinkDownSignalingTime : 1;
	uint32_t Reserved3 : 1;
	uint32_t StationAlias : 1;
	uint32_t Reserved4 : 7;
}ESC_DL_CONTROL, *PESC_DL_CONTROL;

#define ESC_ADR_PHYSICAL_READWRITE_OFFSET 0x0108
typedef uint16_t* PhysicalReadWriteOffset_t;

#define ESC_ADR_ESC_DL_STATUS 0x0110
typedef struct escDLStatus_t
{
	uint16_t PDIOperationalEEPROMLoaded : 1;
	uint16_t PDIWatchdogStatus : 1;
	uint16_t EnhancedLinkDetection : 1;
	uint16_t Reserved : 1;
	uint16_t PhysikalLinkPort0 : 1;
	uint16_t PhysikalLinkPort1 : 1;
	uint16_t PhysikalLinkPort2 : 1;
	uint16_t PhysikalLinkPort3 : 1;
	uint16_t LoopPort0 : 1;
	uint16_t CommunicationPort0 : 1;
	uint16_t LoopPort1 : 1;
	uint16_t CommunicationPort1 : 1;
	uint16_t LoopPort2 : 1;
	uint16_t CommunicationPort2 : 1;
	uint16_t LoopPort3 : 1;
	uint16_t CommunicationPort3 : 1;
}ESC_DL_STATUS,*PESC_DL_STATUS;

#define ESC_ADR_ALCONTROL 0x0120
typedef struct escALControl_t
{
	uint16_t InitiateStateTransition : 4;
	uint16_t ErrorIndAck : 1;
	uint16_t DeviceIdentification : 1;
	uint16_t Reserved : 10;
}AL_CONTROL,*PAL_CONTROL;

#define ESC_ADR_ALSTATUS 0x0130
typedef struct escALStatus_t
{
	uint16_t ActualState : 4;
	uint16_t ErrorInd : 1;
	uint16_t DeviceIdentification : 1;
	uint16_t Reserved : 10;
}AL_STATUS, *PAL_STATUS;

#define ESC_ADR_ALSTATUSCODE 0x0134
typedef uint16_t* escALStatusCode_t;

#define ESC_ADR_RUNLEDOVERRIDE 0x0138
typedef struct escRunLedOverride_t
{
	uint8_t LEDCode : 4;
	uint8_t EnableOverride : 1;
	uint8_t Reserved : 3;
}RUNLED_OVERRIDE, *PRUNLEDOVERRIDE;

#define ESC_ADR_ERRLEDOVERRIDE 0x0139
typedef struct escErrLedOverride_t
{
	uint8_t LEDCode : 4;
	uint8_t EnableOverride : 1;
	uint8_t Reserved : 3;
}ERRLED_OVERRIDE, *PERRLEDOVERRIDE;

#define ESC_ADR_PDI_CONTROL 0x0140
typedef uint8_t* escPDIControl_t;

#define ESC_ADR_ESC_CONFIGURATION 0x0141
typedef struct escEscConfiguration_t
{
	uint8_t DeviceEmulation : 1;
	uint8_t EnhancedLinkDetectionAllPorts : 1;
	uint8_t DistributedClocksSYNCOutUnit : 1;
	uint8_t DistributedClocksLatchInUnit : 1;
	uint8_t EnhancedLinkPort0 : 1;
	uint8_t EnhancedLinkPort1 : 1;
	uint8_t EnhancedLinkPort2 : 1;
	uint8_t EnhancedLinkPort3 : 1;
}ESC_CONFIGURATION,*PESC_CONFIGURATION;

#define ESC_ADR_PDI_INFORMATION 0x014E
typedef struct escPDIInformation_t
{
	uint16_t PDIFunctionAcknowledgeWrite : 1;
	uint16_t ESCConfigurationAreaLoadedFromEEPROM : 1;
	uint16_t PDIActive : 1;
	uint16_t PDIConfigurationInvalid : 1;
	uint16_t Reserved : 12;
}PDI_INFORMATION, *PPDI_INFORMATION;

#define ESC_ADR_PDI_CONFIGURATION 0x0150
typedef uint8_t* escPDIConfiguration;

#define ESC_ADR_SYNCLATCH_PDI_CONFIGURATION 0x0151
typedef uint8_t* escSyncLatchPDIConfiguration;

#define ESC_ADR_PDI_CONFIGURATION 0x0152
typedef uint16_t* escPDIConfigurationExtendend;


