#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
namespace Memory_External {
	DWORD GetProcessID(const wchar_t* procName);
	
	uintptr_t GetModuleBaseAddress(DWORD procID, const wchar_t* modName);
	uintptr_t FindDMAaddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int > offsets);

	void Patch(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void Nop(BYTE* dst, unsigned int size, HANDLE hProcess);
}
namespace Memory_Internal {
	uintptr_t FindDMAaddy(uintptr_t ptr, std::vector<unsigned int > offsets);

	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void Nop(BYTE* dst, unsigned int size);
}
