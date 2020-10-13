#include <Windows.h>
#include <cstdio>
#include <string>

constexpr const char* NOTEPAD_PATH = R"(C:\Windows\System32\notepad.exe)";
constexpr const char* DLL_PATH = "DLL.dll";

constexpr int dll_path_length = std::char_traits<char>::length(DLL_PATH);
constexpr int notepad_path_length = std::char_traits<char>::length(NOTEPAD_PATH);

int main()
{
	DWORD err;
	char path[256];
	char* file_name[64];

	// Get full path of DLL to inject

	const DWORD pathLen = GetFullPathNameA(DLL_PATH, 256, path, file_name);

	// Get LoadLibrary function address –
	// the address doesn't change at remote process

	HMODULE hPath = GetModuleHandleA(path);

	if (!hPath)
	{
		printf("Error with hPath - %lu", GetLastError());
		return -1;
	}

	PVOID addr_load_library =
		static_cast<PVOID>(GetProcAddress(hPath,
			"LoadLibraryA"));

	if (nullptr == addr_load_library)
	{
		printf("%lu", GetLastError());
		return 0;
	}

	// Open remote process
	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 2);
	// Get a pointer to memory location in remote process,
	// big enough to store DLL path
	PVOID memAddr = (PVOID)VirtualAllocEx(proc,
		addr_load_library,
		pathLen,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE
	);
	if (NULL == memAddr) {
		err = GetLastError();
		return 0;
	}
	// Write DLL name to remote process memory
	BOOL check = WriteProcessMemory(proc,
		memAddr,
		path,
		pathLen,
		NULL);

	if (0 == check)
	{
		err = GetLastError();
		return 0;
	}

	// Open remote thread, while executing LoadLibrary
	// with parameter DLL name, will trigger DLLMain
	HANDLE hRemote = CreateRemoteThread(proc,
		NULL,
		0,
		static_cast<LPTHREAD_START_ROUTINE>(addr_load_library),
		NULL,
		0,
		NULL
	);
	if (NULL == hRemote) {

		err = GetLastError();
		return -1;
	}
	WaitForSingleObject(hRemote, INFINITE);
	check = CloseHandle(hRemote);

	if (!check)
	{
		printf("Error in closing handle - %lu\n", GetLastError());
	}

	return 0;
}