#include <Windows.h>
#include <cstdio>

int main()
{
	HANDLE hFile = CreateFileA(
		"gibrish.bin",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	HANDLE hMapFile = CreateFileMappingA(
		hFile,
		NULL,
		PAGE_READWRITE,
		0,
		1,
		"GibrishMap"
	);

	if (!hMapFile)
	{
		printf("Error creating file map - %lu", GetLastError());
		return -1;
	}

	PCHAR pBuf = static_cast<PCHAR>(
		MapViewOfFile(
			hMapFile,
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			1
		)
		);

	if (!pBuf)
	{
		printf("Error mapping view of file - %lu", GetLastError());
		return -1;
	}

	*pBuf = '*';

	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);

	return 0;
}