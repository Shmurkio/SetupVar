#include "SetupVar.h"

bool SetPrivilege(HANDLE HandleToken, LPCTSTR LpszPrivilege, BOOL EnablePrivilege) {

    TOKEN_PRIVILEGES TokenPrivileges;
    LUID Luid;

    if (!LookupPrivilegeValue(nullptr, LpszPrivilege, &Luid)) {
        std::cerr << "LookupPrivilegeValue error: " << GetLastError() << std::endl;
        return false;
    }

    TokenPrivileges.PrivilegeCount = 1;
    TokenPrivileges.Privileges[0].Luid = Luid;

    if (EnablePrivilege) {
        TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    }
    else {
        TokenPrivileges.Privileges[0].Attributes = 0;
    }

    if (!AdjustTokenPrivileges(HandleToken, false, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr)) {
        std::cerr << "AdjustTokenPrivileges error: " << GetLastError() << std::endl;
        return false;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
        std::cerr << "The token does not have the specified privilege." << std::endl;
        return false;
    }

    return true;

}

void RtlInitUnicodeString(PUNICODE_STRING DestinationString, PCWSTR SourceString) {

    if (SourceString) {
        DestinationString->Buffer = const_cast<PWSTR>(SourceString);
        DestinationString->Length = static_cast<USHORT>(wcslen(SourceString) * sizeof(WCHAR));
        DestinationString->MaximumLength = DestinationString->Length + sizeof(WCHAR);
    }
    else {
        DestinationString->Buffer = nullptr;
        DestinationString->Length = 0;
        DestinationString->MaximumLength = 0;
    }

    return;

}

std::wstring GuidToString(const GUID& Guid) {
    wchar_t Buffer[37];

    swprintf(Buffer, sizeof(Buffer) / sizeof(Buffer[0]),
        L"%08lX-%04X-%04X-%04X-%012lX",
        Guid.Data1, Guid.Data2, Guid.Data3,
        (Guid.Data4[0] << 8) | Guid.Data4[1],
        (Guid.Data4[2] << 24) | (Guid.Data4[3] << 16) | (Guid.Data4[4] << 8) | Guid.Data4[5] | (Guid.Data4[6] << 8) | (Guid.Data4[7]));

    return std::wstring(Buffer);
}

std::wstring StringToWString(const std::string& Str) {
    if (Str.empty()) return L"";
    int SizeNeeded = MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, nullptr, 0);
    std::wstring Wstr(SizeNeeded, 0);
    MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, &Wstr[0], SizeNeeded);
    return Wstr;
}

bool InitializeVariable(const char* Argument, std::wstring& VariableCharName, GUID& VariableGuid) {
    std::string VariableCharNameStr;

    if (strcmp(Argument, "-AmiSetup") == 0) {
        VariableCharNameStr = "Setup";
        VariableGuid = gEfiAmiSetupVariableGuid;
    }
    else if (strcmp(Argument, "-InsydeSetup") == 0) {
        VariableCharNameStr = "Setup";
        VariableGuid = gEfiInsydeH2OSetupVariableGuid;
    }
    else if (strcmp(Argument, "-PchSetup") == 0) {
        VariableCharNameStr = "PchSetup";
        VariableGuid = gEfiPchSetupVariableGuid;
    }
    else if (strcmp(Argument, "-SaSetup") == 0) {
        VariableCharNameStr = "SaSetup";
        VariableGuid = gEfiSaSetupVariableGuid;
    }
    else if (strcmp(Argument, "-CpuSetup") == 0) {
        VariableCharNameStr = "CpuSetup";
        VariableGuid = gEfiCpuSetupVariableGuid;
    }
    else if (strcmp(Argument, "-IntelSetup") == 0) {
        VariableCharNameStr = "IntelSetup";
        VariableGuid = gEfiIntelSetupVariableGuid;
    }
    else if (strcmp(Argument, "-SystemConfig") == 0) {
        VariableCharNameStr = "SystemConfig";
        VariableGuid = gEfiSystemConfigVariableGuid;
    }
    else if (strcmp(Argument, "-AmiKek") == 0) {
        VariableCharNameStr = "KEK";
        VariableGuid = gEfiAmiKekVariableGuid;
    }
    else if (strcmp(Argument, "-AmiPk") == 0) {
        VariableCharNameStr = "PK";
        VariableGuid = gEfiAmiPkVariableGuid;
    }
    else {
        return false;
    }

    VariableCharName = StringToWString(VariableCharNameStr);
    return true;
}

void PrintHexData(const BYTE* Data, ULONG Length) {

    for (uint64_t i = 0; i < Length; i += 16) {
        printf("0x%08llXh:  ", i);

        for (uint64_t j = 0; j < 16; ++j) {
            if (i + j < Length) {
                printf("%02X ", static_cast<unsigned int>(Data[i + j]));
            }
            else {
                printf("   ");
            }
        }

        printf("  ");

        for (uint64_t j = 0; j < 16; ++j) {
            if (i + j < Length) {
                char c = Data[i + j];
                printf("%c", std::isprint(c) ? c : '.');
            }
        }

        printf("\n");
    }
}