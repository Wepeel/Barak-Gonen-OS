#include <Windows.h>
#include <cstdio>

constexpr int NUM_PHILS = 5; // For some reason starts to freak out at 65 philosophers

#define EAT() printf("Philosopher %d eating.\n", left_chopstick_index + 1);Sleep(1000)

CRITICAL_SECTION csChopsticks[NUM_PHILS];

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	const int left_chopstick_index = *static_cast<int*>(lpParameter);
	const int right_chopstick_index = ((*static_cast<int*>(lpParameter)) + 1) % NUM_PHILS;

	EnterCriticalSection(csChopsticks + left_chopstick_index);
	EnterCriticalSection(csChopsticks + right_chopstick_index);
	EAT();
	LeaveCriticalSection(csChopsticks + left_chopstick_index);
	LeaveCriticalSection(csChopsticks + right_chopstick_index);

	return 1;
}

int main()
{
	for (size_t i = 0; i < NUM_PHILS; i++)
	{
		InitializeCriticalSection(csChopsticks + i);
	}

	HANDLE hPhilosophers[NUM_PHILS];

	int arr[NUM_PHILS];

	for (int i = 0; i < NUM_PHILS; i++)
	{
		arr[i] = i;
	}

	// Thread Creation
	for (size_t i = 0; i < NUM_PHILS; i++)
	{
		hPhilosophers[i] = CreateThread(
			nullptr,
			NULL,
			ThreadProc,
			arr + i,
			NULL,
			nullptr
		);
	}

	WaitForMultipleObjects(NUM_PHILS, hPhilosophers, TRUE, INFINITE);

	for (size_t i = 0; i < NUM_PHILS; i++)
	{
		DeleteCriticalSection(csChopsticks + i);
	}

	return 0;
}