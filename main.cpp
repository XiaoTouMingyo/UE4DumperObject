#include <windows.h>
#include <iostream>
#include "process.hpp"
#include "Engine.hpp"
using namespace std;

UobjectArray GobjectArray;
DWORD64 GNames = 0;

std::string GetName_New(int key) //新版本算法
{
	DWORD chunkOffset = ((int)(key) >> 16); // Block
	WORD nameOffset = key;
	auto namePoolChunk = Process::ReadProcess<DWORD64>((LPVOID)(GNames + ((chunkOffset + 2) * 8)));
	auto entryOffset = namePoolChunk + (uint32_t)(2 * nameOffset);
	WORD nameLength = Process::ReadProcess<WORD>((PVOID)entryOffset) >> 6;
	if (nameLength > 256)nameLength = 255;
	static char cBuf[256];
	if (Process::ReadMemory((LPVOID)(entryOffset + 2), cBuf, nameLength))
	{
		cBuf[nameLength] = 0;
		return cBuf;
	}
	return std::string();
}

FName GetFName(DWORD64 Address) {

	FName Name = Process::ReadProcess<FName>((PVOID)(Address + 0x18)); //获取FName
	if (Name.ComparisonIndex)
		return Name;
	return FName{};

}

string GetKlassName(DWORD64 Address) {

	DWORD64 UobjectClass = Process::ReadProcess<DWORD64>((PVOID)(Address + 0x10)); //获取UObject->ClassPrivate

	if (UobjectClass == 0)
	{
		return string("");
	}

	string UobjectClassName = GetName_New(GetFName(UobjectClass).ComparisonIndex);

	return UobjectClassName;
}


string GetObjectName(DWORD64 Address) {

	string Name;

	DWORD64 UobjectOuter = Process::ReadProcess<DWORD64>((PVOID)(Address + 0x20)); //获取UObject->OuterPrivate
	if (UobjectOuter != 0)
	{

		for (DWORD64 Outer = UobjectOuter; Outer; Outer = Process::ReadProcess<DWORD64>((PVOID)(Outer + 0x20)))
		{
			Name = GetName_New(GetFName(Outer).ComparisonIndex) + "." + Name;
		}

		Name = GetKlassName(Address) + " " + Name + GetName_New(GetFName(Address).ComparisonIndex);
		return Name;
	}

	Name = GetKlassName(Address) + " " + GetName_New(GetFName(Address).ComparisonIndex);
	return Name;
}


void main() {

	HWND hWnd = FindWindowA("UnrealWindow", NULL); //根据类名获取窗口
	GetWindowThreadProcessId(hWnd, &Process::Pid); //通过窗口获取进程ID
	if (Process::Pid == 0)
	{
		return;
	}

	Process::Phandle =  OpenProcess(PROCESS_ALL_ACCESS, FALSE, Process::Pid); //打开进程

	if (Process::Phandle == 0)
	{
		return;
	}

	GobjectArray = Process::ReadProcess<UobjectArray>((PVOID)((DWORD64)Process::GetProcessMoudleBase() + 0x8031190));
	GNames = (DWORD64)Process::GetProcessMoudleBase() + 0x8011500;

	//int NumElements = Process::ReadProcess<int>((PVOID)(GobjectArray + 0x14));

	cout << "NumElements: " << hex << GobjectArray.NumElements << endl;

	for (int i = 0; i < GobjectArray.NumElements; i++) {
	
		DWORD64 ObjectPtr = GobjectArray.GetObjectPtr(i);
		cout <<"ID:" << i << "       Uobject Address: " << hex << ObjectPtr << "    ObjectName:" << GetObjectName(ObjectPtr) << endl;
	}
}