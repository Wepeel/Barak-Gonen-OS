#include <Windows.h>
#include <cstdio>

constexpr int NUM_PHILS = 64;

// NOTE: This will not work for more than 64 philosophers since the maximum amount of
// object that WaitForMultipleObjects can wait for is 64

#define EAT() printf("Philosopher %d eating.\n", left_chopstick_index + 1)

CRITICAL_SECTION csChopsticks[NUM_PHILS];

DWORD WINAPI ThreadProc(const LPVOID lpParameter)
{
	const int left_chopstick_index = *static_cast<int*>(lpParameter);
	const int right_chopstick_index = ((*static_cast<int*>(lpParameter)) + 1) % NUM_PHILS;

	BOOL success;

	do
	{
		EnterCriticalSection(csChopsticks + left_chopstick_index);
		success = TryEnterCriticalSection(csChopsticks + right_chopstick_index);
		if (success)
		{
			EAT();
			LeaveCriticalSection(csChopsticks + left_chopstick_index);
			LeaveCriticalSection(csChopsticks + right_chopstick_index);
		}

		else
		{
			LeaveCriticalSection(csChopsticks + left_chopstick_index);
		}
	} while (!success);

	return 10;
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
			0,
			NULL,
			ThreadProc,
			arr + i,
			NULL,
			0
		);
	}

	WaitForMultipleObjects(NUM_PHILS, hPhilosophers, TRUE, INFINITE);

	for (size_t i = 0; i < NUM_PHILS; i++)
	{
		DeleteCriticalSection(csChopsticks + i);
	}

	return 0;
}