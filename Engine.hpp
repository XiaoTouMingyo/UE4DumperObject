#include <Windows.h>

struct FName
{
	int ComparisonIndex;
	int Number;
};

struct UObject {
	void** VFTable;
	DWORD ObjectFlags;
	DWORD InternalIndex;
	struct UClass* ClassPrivate;
	FName NamePrivate;
	UObject* OuterPrivate;
};


class UobjectArray {

public:
	void** Objects;
	void* PreAllocatedObjects;
	int MaxElements;
	int NumElements;
	int MaxChunks;
	int NumChunks;

	DWORD64 GetObjectPtr(int id);
};