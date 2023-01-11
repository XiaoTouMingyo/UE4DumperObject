#pragma once
#include <iostream>
#include <Windows.h>
using namespace std;


class Process {

public:
	static DWORD Pid;
	static HANDLE Phandle;
	HMODULE static GetProcessMoudleBase();
	bool static ReadMemory(PVOID address, PVOID buffer, size_t size);
	template <class T> static T ReadProcess(PVOID Address) {

		T Buffer{};
		SIZE_T dwSize = 0;
		ReadProcessMemory(Process::Phandle, Address, &Buffer, sizeof(T), &dwSize);
		return Buffer;

	};
};