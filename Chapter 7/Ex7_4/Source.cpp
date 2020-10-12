#include <Windows.h>
#include <cstdio>

constexpr int SIZE_OF_PRIME_ARRAY = 80000;
constexpr int NUM_PROCESSES = 4;

#define PROCESS_NAME "PrimeProcess\\Release\\PrimeProcess.exe"
constexpr int size = 37 + 3; // Length + 1 space + 1 digit + 1 null terminator

struct prime_help
{
	int arr[SIZE_OF_PRIME_ARRAY] = { 0 };
	int index{};
};

int main()
{

	HANDLE hIndexMutex = CreateMutex(nullptr,
		FALSE,
		TEXT("IndexMutex"));

	HANDLE hMapFile = CreateFileMappingA(
		INVALID_HANDLE_VALUE,
		nullptr,
		PAGE_READWRITE,
		0,
		sizeof(prime_help),
		"Primes"
	);

	char param[size];
	STARTUPINFOA si;
	PROCESS_INFORMATION pi[NUM_PROCESSES];
	for (int i = 0; i < NUM_PROCESSES; i++) {
		sprintf_s(param, size, "%s %d", PROCESS_NAME, i);
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi[i], sizeof(pi[i]));
		BOOL success = CreateProcessA(
			NULL,
			param,
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&si,
			&pi[i]);

		if (!success)
		{
			printf("Failed %lu\n", GetLastError());
		}
	}

	for (int i = 0; i < NUM_PROCESSES; i++)
	{
		WaitForSingleObject(pi[i].hProcess, INFINITE);
	}

	for (int i = 0; i < NUM_PROCESSES; i++)
	{
		CloseHandle(pi[i].hProcess);
		CloseHandle(pi[i].hThread);
	}

	if (!hIndexMutex)
	{
		printf("Error with index mutex\n");
		return -1;
	}

	CloseHandle(hIndexMutex);

	if (!hMapFile)
	{
		printf("Error with map file\n");
		return -1;
	}

	CloseHandle(hMapFile);

	return 0;
}
