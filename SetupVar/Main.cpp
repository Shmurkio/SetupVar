#include "SetupVar.h"

GUID gEfiAmiSetupVariableGuid = EFI_AMI_SETUP_VARIABLE_GUID;
const wchar_t* const gEfiAmiSetupVariableCharName = L"Setup";

int main(int ArgC, char *ArgV[]) {

	if (ArgC != 3) {
		std::cout << "Wrong syntax -> SetupVar.exe Offset Value" << std::endl;
		return 1;
	}

	/* Init variable name & GUID */

	UNICODE_STRING VariableName;
	const wchar_t* const VariableCharName = gEfiAmiSetupVariableCharName;
	GUID VariableGuid = gEfiAmiSetupVariableGuid;

	RtlInitUnicodeString(&VariableName, VariableCharName);

	/* Open the process token */

	HANDLE HandleToken;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &HandleToken)) {
		std::cerr << "OpenProcessToken error: " << GetLastError() << std::endl;
		return 1;
	}

	/* Enable the SeSystemEnvironmentPrivilege privilege */

	if (!SetPrivilege(HandleToken, SE_SYSTEM_ENVIRONMENT_NAME, TRUE)) {
		std::cerr << "Failed to enable SeSystemEnvironmentPrivilege" << std::endl;
		CloseHandle(HandleToken);
		return 1;
	}

	/* Prepare the buffer of the variable data to receive the value */

	ULONG VariableLength = 4096;
	BYTE VariableData[4096];
	ULONG Attributes;

	/* Call the NtQuerySystemEnvironmentValueEx function */

	NTSTATUS Status = NtQuerySystemEnvironmentValueEx(&VariableName, &VariableGuid, VariableData, &VariableLength, &Attributes);

	if (Status != STATUS_SUCCESS) {
		std::cerr << "NVRAM variable can't be read. Status: 0x" << std::hex << Status << std::endl;
		CloseHandle(HandleToken);
		return static_cast<int>(Status);
	}

	/* Patch the VariableData */

	int Offset;
	BYTE Value;

	try {
		Offset = std::stoi(ArgV[1], nullptr, 16);
		unsigned long parsedValue = std::stoul(ArgV[2], nullptr, 16);

		if (parsedValue > 0xFF) {
			std::cerr << "Value out of bounds" << std::endl;
			CloseHandle(HandleToken);
			return 1;
		}

		Value = static_cast<BYTE>(parsedValue);
	}
	catch (const std::invalid_argument&) {
		std::cerr << "Invalid offset or value format" << std::endl;
		CloseHandle(HandleToken);
		return 1;
	}
	catch (const std::out_of_range&) {
		std::cerr << "Offset or value out of range" << std::endl;
		CloseHandle(HandleToken);
		return 1;
	}

	if (Offset < 0 || Offset >= static_cast<int>(VariableLength)) {
		std::cerr << "Offset out of bounds" << std::endl;
		CloseHandle(HandleToken);
		return 1;
	}

	BYTE OrigValue = VariableData[Offset];
	VariableData[Offset] = Value;

	/* Delete NVRAM variable */

	Status = NtSetSystemEnvironmentValueEx(&VariableName, &VariableGuid, 0, 0, Attributes);

	if (Status != STATUS_SUCCESS) {
		std::cerr << "Failed to delete NVRAM variable. Status: 0x" << std::hex << Status << std::endl;
		CloseHandle(HandleToken);
		return static_cast<int>(Status);
	}

	/* Set modified variable */

	Status = NtSetSystemEnvironmentValueEx(&VariableName, &VariableGuid, VariableData, VariableLength, Attributes);

	if (Status != STATUS_SUCCESS) {
		std::cerr << "Failed to set NVRAM variable. Status: 0x" << std::hex << Status << std::endl;
	}
	else {
		std::cerr << "Set NVRAM variable: 0x" << std::hex << std::uppercase << static_cast<int>(OrigValue) << " -> 0x" << static_cast<int>(VariableData[Offset]) << std::endl;
	}

	CloseHandle(HandleToken);

	return static_cast<int>(Status);

}