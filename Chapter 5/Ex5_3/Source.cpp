#include <Windows.h>
#include <cstdio>

#define EXE_FILENAME "SinglePhilosopher\\Debug\\SinglePhilosopher.exe"
#define NUM_PROCESSES 5
constexpr INT size = 48; // length is increased by 3:
// space character- 1 byte,
	// one digit number- 1 byte,
	// string NULL terminator- 1 byte
constexpr INT length_mutex_name = 11; // strlen(Chopstick0) + null

int main()
{
	HANDLE hPhilosophers[NUM_PROCESSES];
	char mutex_name[length_mutex_name];

	for (size_t i = 0; i < NUM_PROCESSES; i++)
	{
		sprintf_s(mutex_name, length_mutex_name, "Chopstick%zu", i);
		hPhilosophers[i] = CreateMutexA(NULL, FALSE, mutex_name);
	}

	char param[size];
	STARTUPINFOA si;
	PROCESS_INFORMATION pi[NUM_PROCESSES];
	for (int i = 0; i < NUM_PROCESSES; i++) {
		sprintf_s(param, size, "%s %d", EXE_FILENAME, i);
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
			printf("Failed\n");
		}
	}

	for (int i = 0; i < NUM_PROCESSES; i++) {
		WaitForSingleObject(pi[i].hProcess, INFINITE);
	}

	for (size_t i = 0; i < NUM_PROCESSES; i++)
	{
		ReleaseMutex(hPhilosophers[i]);
	}

	for (int i = 0; i < NUM_PROCESSES; i++)
	{
		CloseHandle(pi[i].hProcess);
		CloseHandle(pi[i].hThread);
	}

	return 0;
}