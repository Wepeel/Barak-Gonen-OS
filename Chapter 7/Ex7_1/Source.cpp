#include <Windows.h>
#include <cstdio>

int main()
{
	HANDLE hFile = CreateFile(TEXT("gibrish.bin"),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	char read_char;
	DWORD chars_read;
	int As = 0;
	BOOL success = false;

	do
	{
		if (FALSE != ReadFile(hFile,
			&read_char,
			1,
			&chars_read,
			NULL))
		{
			if (read_char == 'A')
			{
				As++;
			}
		}

		else
		{
			success = true;
		}

		//printf("%c", read_char);


	} while (!success);

	printf("%d\n", As);



	return 0;
}