#include <Windows.h>
#include <cstdio>

constexpr int SIZE_OF_PRIME_ARRAY = 80000;

struct prime_help
{
	int arr[SIZE_OF_PRIME_ARRAY];
	int index;
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

	if (starting_index == 1) {
		//cout << starting_index << " ";
		WaitForSingleObject(hIndexMutex, INFINITE);
		help->arr[help->index] = starting_index;
		help->index++;
		ReleaseMutex(hIndexMutex);
		starting_index++;
		if (ending_index >= 2) {
			//cout << starting_index << " ";
			WaitForSingleObject(hIndexMutex, INFINITE);
			help->arr[help->index] = starting_index;
			help->index++;
			ReleaseMutex(hIndexMutex);
			starting_index++;
		}
	}
	if (starting_index == 2)
	{
		WaitForSingleObject(hIndexMutex, INFINITE);
		help->arr[help->index] = starting_index;
		help->index++;
		ReleaseMutex(hIndexMutex);
	}
	//cout << starting_index << " ";


	if (starting_index % 2 == 0)
		starting_index++;

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
			WaitForSingleObject(hIndexMutex, INFINITE);
			help->arr[help->index] = i;
			help->index++;
			ReleaseMutex(hIndexMutex);
		}
		//cout << i << " ";
	}

	if (!help)
	{
		printf("NULL ptr");
		return -1;
	}

	for (int k = 0; k < 80000; k++)
	{
		if (0 != help->arr[k])
		{
			printf("%d\n", help->arr[k]);
		}
	}

	CloseHandle(hMapFile);
	CloseHandle(hIndexMutex);

	return 0;
}