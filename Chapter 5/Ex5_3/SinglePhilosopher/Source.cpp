#include <Windows.h>
#include <cstdio>

#define EAT() printf("Philosopher %d eating.\n", philosopher_index)

int main(int argc, char* argv[])
{
	if (argc != 2) {
		printf("Incorrect number of arguments\n");
		return 1;
	}

	const int philosopher_index = atoi(argv[1]);

	constexpr size_t length = 11;

	char mutex1_buf[length];
	char mutex2_buf[length];

	sprintf_s(mutex1_buf, length, "Chopstick%d", philosopher_index);
	sprintf_s(mutex2_buf, length, "Chopstick%d", philosopher_index + 1 % 5);

	HANDLE hChopstick1 = CreateMutexA(nullptr, FALSE, mutex1_buf);
	HANDLE hChopstick2 = CreateMutexA(nullptr, FALSE, mutex2_buf);

	if (hChopstick1 && hChopstick2)
	{
		BOOL success = false;

		do
		{
			WaitForSingleObject(hChopstick1, INFINITE);
			DWORD wait_result = WaitForSingleObject(hChopstick2, 500);

			if (wait_result == WAIT_OBJECT_0)
			{
				EAT();
				ReleaseMutex(hChopstick1);
				ReleaseMutex(hChopstick2);
				success = true;
			}

			else
			{
				ReleaseMutex(hChopstick1);
			}
		} while (!success);

		CloseHandle(hChopstick1);
		CloseHandle(hChopstick2);
	}

	return 0;
}