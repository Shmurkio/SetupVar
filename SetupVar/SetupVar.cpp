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