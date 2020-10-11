#include <Windows.h>
#include <cstdio>

#define SEARCH_LETTER "A"

int CountChar(PCHAR pBuf, int buff_size, LPCSTR letter);

int main()
{
	// get system memory allignement granularity (usually 65536)
	SYSTEM_INFO sys_info;
	GetSystemInfo(&sys_info);
	int mem_buffer_size = sys_info.dwAllocationGranularity;
	// open an existing file for reading
	HANDLE hFile;
	LPCSTR pFileName = "gibrish.bin";
	hFile = CreateFileA(
		pFileName, // file name
		GENERIC_READ, // access type
		0, // other processes can't share
		NULL, // security
		OPEN_EXISTING, // open only if file exists
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	DWORD file_size = GetFileSize(hFile, NULL);
	// create file mapping object
	HANDLE hMapFile;
	hMapFile = CreateFileMappingA(
		hFile, // file handle
		NULL, // default security
		PAGE_READONLY, // read access
		0, // maximum object size (high-order
		// DWORD)
		0, // maximum object size (low-order DWORD)
		// 0 means map the whole file
		"myFile"); // name of mapping object, in case we
		// want to share it
		// read the file, one page at a time
	int buffer_number = 0, count = 0;
	int file_location = buffer_number * mem_buffer_size;
	LPSTR pBuf;
	LPCSTR letter = SEARCH_LETTER;
	while (file_location <= (file_size - mem_buffer_size)) {
		pBuf = (LPSTR)MapViewOfFile(
			hMapFile, // handle to map object
			FILE_MAP_READ, // read/write permission
			0, // start point (upper word)
			file_location, // start point (lower word)
			mem_buffer_size); // how many bytes to read
		count += CountChar(pBuf, mem_buffer_size, letter);
		buffer_number++;
		file_location = mem_buffer_size * buffer_number;
		UnmapViewOfFile(pBuf);
		Sleep(100);
	}
	int reminder = file_size - file_location;
	pBuf = (LPSTR)MapViewOfFile(
		hMapFile, // handle to map object
		FILE_MAP_READ, // read/write permission
		0, // start point (upper word)
		file_location, // start point (lower word)
		reminder); // how many bytes to read
	count = CountChar(pBuf, reminder, letter);
	printf("%d\n", count);
	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);
	CloseHandle(hFile);
	return 0;
}

int CountChar(PCHAR pBuf, int buff_size, LPCSTR letter) {
	static int count = 0;
	for (int i = 0; i < buff_size; i++) {
		if (pBuf[i] == *letter) count++;
	}
	return count;
}