#include "process.hpp"
#include <TlHelp32.h>

DWORD Process::Pid = 0;
HANDLE Process::Phandle = 0;

HMODULE Process::GetProcessMoudleBase() {
	HANDLE handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Process::Pid); //  获取进程的所有的模块。
	if (handle == 0)
	{
		return NULL;
	}


	MODULEENTRY32 moduleEntry = { 0 };
	moduleEntry.dwSize = sizeof(MODULEENTRY32);

	if (!Module32First(handle, &moduleEntry)) { //获取进程第一个模块
		CloseHandle(handle);
		return NULL;
	}

	CloseHandle(handle);
	return moduleEntry.hModule;
}


bool Process::ReadMemory(PVOID address, PVOID buffer, size_t size)
{
	SIZE_T ret_size;
	return ReadProcessMemory(Process::Phandle, address, buffer, size, &ret_size);
}