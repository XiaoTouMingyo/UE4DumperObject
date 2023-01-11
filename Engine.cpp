#include "Engine.hpp"
#include "Process.hpp"


DWORD64 UobjectArray::GetObjectPtr(int id) {

	if (id >= NumElements) return 0;
	DWORD64 chunkIndex = id / 65536; //chunkIndex = 0
	if (chunkIndex >= NumChunks) return 0;
	auto chunk = Process::ReadProcess<void*>(Objects+(chunkIndex * 8));//
	if (!chunk) return 0;
	DWORD withinChunkIndex = id % 65536 * 32;
	auto item = Process::ReadProcess<PVOID>((PVOID)((DWORD64)chunk + withinChunkIndex));
	return (DWORD64)item;

}