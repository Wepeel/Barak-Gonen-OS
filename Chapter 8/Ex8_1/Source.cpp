#include <Windows.h>
#include <cstdio>
#include <string>

constexpr int notepad_pid = 9692;

constexpr const char* NOTEPAD_PATH = R"(C:\Windows\System32\notepad.exe)";
constexpr const char* DLL_PATH = "..\\DLL\\x64\\Debug\\DLL.dll";

constexpr int dll_path_length = std::char_traits<char>::length(DLL_PATH);
constexpr int notepad_path_length = std::char_traits<char>::length(NOTEPAD_PATH);

int main()
{
	char path[256]{ 0 };

	// Get full path of DLL to inject

	GetFullPathNameA(DLL_PATH, 255, path, NULL);
	const DWORD pathLen = strlen(path);

	// Get LoadLibrary function address –
	// the address doesn't change at remote process

	HMODULE hPath = GetModuleHandleA("kernel32.dll");

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
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, notepad_pid);
	if (NULL == hProc)
	{
		printf("Error in opening process - %lu\n", GetLastError());
		return -1;
	}
	// Get a pointer to memory location in remote process,
	// big enough to store DLL path
	PVOID memAddr = static_cast<PVOID>(VirtualAllocEx(hProc,
		NULL,
		pathLen,
		MEM_RESERVE | MEM_COMMIT,
		PAGE_READWRITE
	));

	if (nullptr == memAddr)
	{
		printf("Error in allocation virtual memory - %lu\n", GetLastError());
		return -1;
	}
	// Write DLL name to remote process memory
	BOOL check = WriteProcessMemory(hProc,
		memAddr,
		path,
		pathLen,
		NULL);

	if (0 == check)
	{
		printf("Error in writing process memory - %lu\n", GetLastError());
		return -1;
	}

	// Open remote thread, while executing LoadLibrary
	// with parameter DLL name, will trigger DLLMain
	HANDLE hRemote = CreateRemoteThread(hProc,
		NULL,
		0,
		static_cast<LPTHREAD_START_ROUTINE>(addr_load_library),
		memAddr,
		0,
		NULL
	);
	if (NULL == hRemote) {

		printf("Error in creating remote thread - %lu\n", GetLastError());
		return -1;
	}
	WaitForSingleObject(hRemote, INFINITE);
	check = CloseHandle(hRemote);

	if (!check)
	{
		printf("Error in closing handle - %lu\n", GetLastError());
		return -1;
	}

	return 0;
}