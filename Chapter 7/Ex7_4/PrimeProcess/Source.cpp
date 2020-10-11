#include <Windows.h>
#include <cstdio>

int main(int argc, char* argv[])
{
	if (2 != argc)
	{
		printf("NO. Bad Args");
		return -1;
	}

	const int proc_number = atoi(argv[1]);

	int starting_index = 250000 * proc_number;
	int ending_index = starting_index + 24999;

	int i, j;

	if (starting_index == 1) {
		//cout << starting_index << " ";
		starting_index++;
		if (ending_index >= 2) {
			//cout << starting_index << " ";
			starting_index++;
		}
	}
	if (starting_index == 2)
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
		if (flag == 1) {}
		//cout << i << " ";
	}

	return 0;
}