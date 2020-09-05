#include <Windows.h>
#include <stdio.h>

DWORD WINAPI print_1000(LPVOID param)
{
	for (INT i = 1; i <= 1000; i++)
	{
		printf("Thread %d: %d\n", *(LPINT)param, i);
	}

	return 1;
}

int main()
{

	int array[] = { 1,2,3,4 };

	HANDLE hThreads[4];

	hThreads[0] = CreateThread(
		NULL,
		0,
		print_1000,
		array,
		0,
		NULL
	);

	hThreads[1] = CreateThread(
		NULL,
		0,
		print_1000,
		array + 1,
		0,
		NULL
	);

	hThreads[2] = CreateThread(
		NULL,
		0,
		print_1000,
		array + 2,
		0,
		NULL
	);

	hThreads[3] = CreateThread(
		NULL,
		0,
		print_1000,
		array + 3,
		0,
		NULL
	);

	WaitForMultipleObjects(4, hThreads, TRUE, INFINITE);

	return 0;
}