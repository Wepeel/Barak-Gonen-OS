#include <Windows.h>

CRITICAL_SECTION csChopsticks[5];

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	BOOL success = TryEnterCriticalSection(csChopsticks);

	if (success)
	{
		// Place critical code here
		LeaveCriticalSection(csChopsticks);
	}
	return 1;
}

int main()
{
	for (size_t i = 0; i < 5; i++)
	{
		InitializeCriticalSection(csChopsticks + i);
	}

	HANDLE hPhilosophers[5];

	const int arr[] = { 1,2,3,4,5 };

	for()

	for (size_t i = 0; i < 5; i++)
	{
		DeleteCriticalSection(csChopsticks + i);
	}

	return 0;
}