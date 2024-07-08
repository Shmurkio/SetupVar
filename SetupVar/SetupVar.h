#pragma once

#include <iostream>
#include <Windows.h>
#include <winternl.h>
#include <ntstatus.h>
#include <string>
#include <iomanip>

/* gRT->Getvariable communication */

extern "C" NTSTATUS NTAPI NtQuerySystemEnvironmentValueEx(PUNICODE_STRING VariableName, LPGUID VendorGuid, PVOID Value, PULONG ValueLength, PULONG Attributes);

/* gRT->SetVariable communication */

extern "C" NTSTATUS NTAPI NtSetSystemEnvironmentValueEx(PUNICODE_STRING VariableName, LPGUID VendorGuid, PVOID Value, ULONG ValueLength, ULONG Attributes);

/* Initializes variable  */

bool InitializeVariable(const char* Argument, std::wstring& VariableCharName, GUID& VariableGuid);

/* Set privilege */

bool SetPrivilege(HANDLE HandleToken, LPCTSTR LpszPrivilege, BOOL EnablePrivilege);

/* Init unicode string */

void RtlInitUnicodeString(PUNICODE_STRING DestinationString, PCWSTR SourceString);

/* Convert GUID to string */

std::wstring GuidToString(GUID Guid);

/* Print variable data */

void PrintHexData(const BYTE* Data, ULONG Length);

/* GUIDs */

#define EFI_AMI_SETUP_VARIABLE_GUID {0xEC87D643, 0xEBA4, 0x4BB5, {0xA1, 0xE5, 0x3F, 0x3E, 0x36, 0xB2, 0x0D, 0xA9}}
#define EFI_INSYDEH2O_SETUP_VARIABLE_GUID {0xa04A27F4, 0xDF00, 0x4D42, {0xB5, 0x52, 0x39, 0x51, 0x13, 0x02, 0x11, 0x3D}}
#define EFI_PCH_SETUP_VARIABLE_GUID {0x4570B7F1, 0xADE8, 0x4943, {0x8D, 0xC3, 0x40, 0x64, 0x72, 0x84, 0x23, 0x84}}
#define EFI_SA_SETUP_VARIABLE_GUID {0x72C5E28C, 0x7783, 0x43A1, {0x87, 0x67, 0xFA, 0xD7, 0x3F, 0xCC, 0xAF, 0xA4}}
#define EFI_CPU_SETUP_VARIABLE_GUID {0xB08F97FF, 0xE6E8, 0x4193, {0xA9, 0x97, 0x5E, 0x9E, 0x9B, 0x0A, 0xDB, 0x32}}
#define EFI_INTEL_SETUP_VARIABLE_GUID {0xEC87D643, 0xEBA4, 0x4BB5, {0xA1, 0xE5, 0x3F, 0x3E, 0x36, 0xB2, 0x0D, 0xA9}}
#define EFI_SYSTEM_CONFIG_VARIABLE_GUID {0xA04A27F4, 0xDF00, 0x4D42, {0xB5, 0x52, 0x39, 0x51, 0x13, 0x02, 0x11, 0x3D}}
#define EFI_AMI_KEK_VARIABLE_GUID {0x8BE4DF61, 0x93CA, 0x11D2, {0xAA, 0x0D, 0x00, 0xE0, 0x98, 0x03, 0x2B, 0x8C}}
#define EFI_AMI_PK_VARIABLE_GUID {0x8BE4DF61, 0x93CA, 0x11D2, {0xAA, 0x0D, 0x00, 0xE0, 0x98, 0x03, 0x2B, 0x8C}}

extern GUID gEfiAmiSetupVariableGuid;
extern GUID gEfiInsydeH2OSetupVariableGuid;
extern GUID gEfiPchSetupVariableGuid;
extern GUID gEfiSaSetupVariableGuid;
extern GUID gEfiCpuSetupVariableGuid;
extern GUID gEfiIntelSetupVariableGuid;
extern GUID gEfiSystemConfigVariableGuid;
extern GUID gEfiAmiKekVariableGuid;
extern GUID gEfiAmiPkVariableGuid;