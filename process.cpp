#include "process.hpp"
#include <TlHelp32.h>

DWORD Process::Pid = 0;
HANDLE Process::Phandle = 0;

HMODULE Process::GetProcessMoudleBase() {
	HANDLE handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Process::Pid); //  ��ȡ���̵����е�ģ�顣
	if (handle == 0)
	{
		return NULL;
	}


	MODULEENTRY32 moduleEntry = { 0 };
	moduleEntry.dwSize = sizeof(MODULEENTRY32);

	if (!Module32First(handle, &moduleEntry)) { //��ȡ���̵�һ��ģ��
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