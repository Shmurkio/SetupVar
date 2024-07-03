#pragma once

#include <iostream>
#include <Windows.h>
#include <winternl.h>
#include <ntstatus.h>
#include <string>

/* gRT->Getvariable communication */

extern "C" NTSTATUS NTAPI NtQuerySystemEnvironmentValueEx(PUNICODE_STRING VariableName, LPGUID VendorGuid, PVOID Value, PULONG ValueLength, PULONG Attributes);

/* gRT->SetVariable communication */

extern "C" NTSTATUS NTAPI NtSetSystemEnvironmentValueEx(PUNICODE_STRING VariableName, LPGUID VendorGuid, PVOID Value, ULONG ValueLength, ULONG Attributes);

/* Set privilege */

bool SetPrivilege(HANDLE HandleToken, LPCTSTR LpszPrivilege, BOOL EnablePrivilege);

/* Init unicode string */

void RtlInitUnicodeString(PUNICODE_STRING DestinationString, PCWSTR SourceString);

/* Setup variable data (AMI) */

#define EFI_AMI_SETUP_VARIABLE_GUID {0xEC87D643, 0xEBA4, 0x4BB5, {0xA1, 0xE5, 0x3F, 0x3E, 0x36, 0xB2, 0x0D, 0xA9}}
extern GUID gEfiAmiSetupVariableGuid;
extern const wchar_t* const gEfiAmiSetupVariableCharName;