#include "SetupVar.h"

GUID gEfiAmiSetupVariableGuid = EFI_AMI_SETUP_VARIABLE_GUID;
GUID gEfiInsydeH2OSetupVariableGuid = EFI_INSYDEH2O_SETUP_VARIABLE_GUID;
GUID gEfiPchSetupVariableGuid = EFI_PCH_SETUP_VARIABLE_GUID;
GUID gEfiSaSetupVariableGuid = EFI_SA_SETUP_VARIABLE_GUID;
GUID gEfiCpuSetupVariableGuid = EFI_CPU_SETUP_VARIABLE_GUID;
GUID gEfiIntelSetupVariableGuid = EFI_INTEL_SETUP_VARIABLE_GUID;
GUID gEfiSystemConfigVariableGuid = EFI_SYSTEM_CONFIG_VARIABLE_GUID;
GUID gEfiAmiKekVariableGuid = EFI_AMI_KEK_VARIABLE_GUID;
GUID gEfiAmiPkVariableGuid = EFI_AMI_PK_VARIABLE_GUID;

int main(int ArgC, char *ArgV[]) {

    /* Print header */

    system("CLS");

    printf("################################################################################\n");
    printf("#                                                                              #\n");
    printf("#                                  SetupVar 1.2                                #\n");
    printf("#                                   by Shmurkio                                #\n");
    printf("#                                                                              #\n");
    printf("################################################################################\n\n");


    /* Check Argument count */

    if (ArgC != 2 && ArgC != 4) {
        printf("Syntax:\n");
        printf("SetupVar -VariableName Offset Value    => Edit variable data\n");
        printf("SetupVar -VariableName                 => Print variable data\n\n");
        printf("Variable names:\n");
        wprintf(L"- AmiSetup       (%s)\n", GuidToString(gEfiAmiSetupVariableGuid).c_str());
        wprintf(L"- InsydeSetup    (%s)\n", GuidToString(gEfiInsydeH2OSetupVariableGuid).c_str());
        wprintf(L"- PchSetup       (%s)\n", GuidToString(gEfiPchSetupVariableGuid).c_str());
        wprintf(L"- SaSetup        (%s)\n", GuidToString(gEfiSaSetupVariableGuid).c_str());
        wprintf(L"- CpuSetup       (%s)\n", GuidToString(gEfiCpuSetupVariableGuid).c_str());
        wprintf(L"- IntelSetup     (%s)\n", GuidToString(gEfiIntelSetupVariableGuid).c_str());
        wprintf(L"- SystemConfig   (%s)\n", GuidToString(gEfiSystemConfigVariableGuid).c_str());
        wprintf(L"- AmiKek         (%s)\n", GuidToString(gEfiAmiKekVariableGuid).c_str());
        wprintf(L"- AmiPk          (%s)\n", GuidToString(gEfiAmiPkVariableGuid).c_str());
        return 1;
    }

    /* Init variable name & GUID */

    UNICODE_STRING VariableName;
    std::wstring VariableCharName;
    GUID VariableGuid;

    if (!InitializeVariable(ArgV[1], VariableCharName, VariableGuid)) {
        printf("Variable \"%s\" is not defined.", ArgV[1]);
        return 2;
    }

    VariableName.Buffer = const_cast<wchar_t*>(VariableCharName.c_str());
    VariableName.Length = static_cast<USHORT>(VariableCharName.length() * sizeof(wchar_t));
    VariableName.MaximumLength = VariableName.Length + sizeof(wchar_t);

    /* Open the process token */

    HANDLE HandleToken;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &HandleToken)) {
        printf("OpenProcessToken error: %lu\n.", GetLastError());
        return static_cast<int>(GetLastError());
    }

    /* Enable the SeSystemEnvironmentPrivilege privilege */

    if (!SetPrivilege(HandleToken, SE_SYSTEM_ENVIRONMENT_NAME, TRUE)) {
        printf("Failed to enable SeSystemEnvironmentPrivilege.\n");
        CloseHandle(HandleToken);
        return 3;
    }

    /* Prepare the buffer of the variable data to receive the value */

    ULONG VariableLength = 4096;
    BYTE VariableData[4096];
    ULONG Attributes;

    /* Call the NtQuerySystemEnvironmentValueEx function */

    NTSTATUS Status = NtQuerySystemEnvironmentValueEx(&VariableName, &VariableGuid, VariableData, &VariableLength, &Attributes);

    if (Status != STATUS_SUCCESS) {
        printf("NVRAM variable can't be read. Status: 0x%X.\n", Status);
        return static_cast<int>(Status);
    }

    /* Check if it should only print the variable (3 args) */

    if (ArgC == 2) {
        wprintf(L"Name: %s\n", VariableName.Buffer);
        wprintf(L"GUID: %s\n", GuidToString(VariableGuid).c_str());
        printf("Size: 0x%X\n\n", VariableLength);
        PrintHexData(VariableData, VariableLength);
        CloseHandle(HandleToken);
        return 0;
    }

    /* Execute the command */

    int Offset;
    BYTE Value;

    try {
        Offset = std::stoi(ArgV[2], nullptr, 16);
        unsigned long ParsedValue = std::stoul(ArgV[3], nullptr, 16);

        if (ParsedValue > 0xFF) {
            printf("Value out of bounds.\n");
            CloseHandle(HandleToken);
            return 4;
        }

        Value = static_cast<BYTE>(ParsedValue);
    }
    catch (const std::invalid_argument&) {
        printf("Invalid offset or value format.\n");
        CloseHandle(HandleToken);
        return 5;
    }
    catch (const std::out_of_range&) {
        printf("Offset or value out of range.\n");
        CloseHandle(HandleToken);
        return 6;
    }

    if (Offset < 0 || Offset >= static_cast<int>(VariableLength)) {
        printf("Offset out of bounds.\n");
        CloseHandle(HandleToken);
        return 7;
    }

    BYTE OrigValue = VariableData[Offset];
    VariableData[Offset] = Value;

    /* Delete NVRAM variable */

    Status = NtSetSystemEnvironmentValueEx(&VariableName, &VariableGuid, nullptr, 0, Attributes);

    if (Status != STATUS_SUCCESS) {
        printf("Failed to delete NVRAM variable. Status: 0x%X.\n", Status);
        CloseHandle(HandleToken);
        return static_cast<int>(Status);
    }

    /* Set modified variable */

    Status = NtSetSystemEnvironmentValueEx(&VariableName, &VariableGuid, VariableData, VariableLength, Attributes);

    if (Status != STATUS_SUCCESS) {
        printf("Failed to set NVRAM variable. Status: 0x%X.\n", Status);
        CloseHandle(HandleToken);
        return static_cast<int>(Status);
    }

    wprintf(L"\"%s\", %s, 0x%X: 0x%X -> 0x%X\n", GuidToString(VariableGuid).c_str(), VariableName.Buffer, Offset, OrigValue, Value);

    CloseHandle(HandleToken);

    return 0;

}