#include "Memory.h"

DWORD Memory_External::GetProcessID(const wchar_t* procName) {
	std::cout << "Getting Process ID..." << '\n';
	DWORD procID = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry)) {
			do {
				if (!_wcsicmp(procEntry.szExeFile, procName)) {
					procID = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procID;
}
uintptr_t Memory_External::GetModuleBaseAddress(DWORD procID, const wchar_t* modName) {
	std::cout << "Getting Module Base Address..." << '\n';
	uintptr_t moduBaseAddress = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!_wcsicmp(modEntry.szModule, modName)) {
					moduBaseAddress = (uintptr_t)modEntry.modBaseAddr;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return moduBaseAddress;
}
uintptr_t Memory_External::FindDMAaddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int > offsets) {
	std::cout << "Getting Finding DMA Addy..." << '\n';
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i) {
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

void  Memory_External::Patch(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess) {
	DWORD oldportect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldportect);
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	VirtualProtectEx(hProcess, dst, size, oldportect, &oldportect);
}
void  Memory_External::Nop(BYTE* dst, unsigned int size, HANDLE hProcess) {
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);

	Memory_External::Patch(dst, nopArray, size, hProcess);
	delete[] nopArray;
}
//

//

//
void  Memory_Internal::Patch(BYTE* dst, BYTE* src, unsigned int size) {
	DWORD oldportect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldportect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldportect, &oldportect);
}
void  Memory_Internal::Nop(BYTE* dst, unsigned int size) {
	DWORD oldportect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldportect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldportect, &oldportect);
}
uintptr_t Memory_Internal::FindDMAaddy(uintptr_t ptr, std::vector<unsigned int > offsets) {
	std::cout << "Getting Finding DMA Addy..." << '\n';
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i) {

		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}