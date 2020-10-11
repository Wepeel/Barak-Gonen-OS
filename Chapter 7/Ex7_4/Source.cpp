#include <Windows.h>
#include <cstdio>

constexpr int SIZE_OF_PRIME_ARRAY = 80000;
constexpr int NUM_PROCESSES = 4;

#define PROCESS_NAME "PrimeProcess\\Debug\\PrimeProcess.exe"
constexpr int size = 35 + 3; // Length + 1 space + 1 digit + 1 null terminator

struct prime_help
{
	int arr[SIZE_OF_PRIME_ARRAY];
	int index;
};

int main()
{
	prime_help primes{};

	HANDLE hIndexMutex = CreateMutex(nullptr,
		FALSE,
		TEXT("IndexMutex"));

	HANDLE hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		nullptr,
		PAGE_READWRITE,
		0,
		sizeof(prime_help),
		TEXT("Primes")
	);

	char param[size];
	STARTUPINFOA si;
	PROCESS_INFORMATION pi[NUM_PROCESSES];
	for (int i = 0; i < NUM_PROCESSES; i++) {
		sprintf_s(param, size, "%s %d", PROCESS_NAME, i);
		//sprintf_s(param, size, "%s", PROCESS_NAME);
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
			printf("Failed %d\n", GetLastError());
		}
	}

	CloseHandle(hIndexMutex);
	CloseHandle(hMapFile);


	return 0;
}
