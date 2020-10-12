#include <Windows.h>
#include <cstdio>

constexpr int SIZE_OF_PRIME_ARRAY = 80000;

#define ADD_PRIME(x) WaitForSingleObject(hIndexMutex, INFINITE);\
help->arr[help->index] = x;\
help->index++;\
ReleaseMutex(hIndexMutex);

struct prime_help
{
	int arr[SIZE_OF_PRIME_ARRAY] = { 0 };
	int starting_range{ 0 };
	int index{ 0 };
};

int main(int argc, char* argv[])
{

	prime_help* help;

	if (2 != argc)
	{
		printf("NO. Bad Args");
		return -1;
	}

	HANDLE hIndexMutex = CreateMutex(nullptr,
		FALSE,
		TEXT("IndexMutex"));

	HANDLE hMapFile = OpenFileMappingA(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		"Primes"
	);

	if (!hMapFile)
	{
		printf("%lu", GetLastError());
		return 0;
	}

	help = static_cast<prime_help*>(
		MapViewOfFile(
			hMapFile,
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			sizeof(prime_help)
		));

	const int proc_number = atoi(argv[1]);

	int starting_index = 250000 * proc_number;
	int ending_index = starting_index + 249999;

	int i, j;

	if (0 == starting_index)
	{
		starting_index = 2;
	}

	if (1 == starting_index)
	{
		//cout << starting_index << " ";

		starting_index++;
		if (ending_index >= 2) {
			//cout << starting_index << " ";
			ADD_PRIME(starting_index);
			starting_index++;
		}
	}
	if (starting_index == 2)
	{
		ADD_PRIME(starting_index)
	}

	if (starting_index % 2 == 0)
	{
		starting_index++;
	}

	for (i = starting_index; i <= ending_index; i = i + 2) {

		bool flag = 1;

		for (j = 2; j * j <= i; ++j) {
			if (i % j == 0) {
				flag = 0;
				break;
			}
		}

		// flag = 1 means i is prime 
		// and flag = 0 means i is not prime 
		if (flag == 1)
		{
			ADD_PRIME(i)
		}
		//cout << i << " ";
	}

	if (!help)
	{
		printf("NULL ptr");
		return -1;
	}

	if (3 == proc_number)
	{


		for (int k = 0; k < 80000; k++)
		{
			if (0 != help->arr[k])
			{
				printf("%d\n", help->arr[k]);
			}
		}
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