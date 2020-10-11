#include <Windows.h>
#include <cstdio>

#define BYTES_TO_READ 64

int main()
{
	HANDLE hFile = CreateFile(TEXT("gibrish.bin"),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	char chars[BYTES_TO_READ];
	DWORD chars_read;
	int As = 0;
	BOOL done = false;

	do
	{
		if (FALSE != ReadFile(hFile,
			chars,
			BYTES_TO_READ,
			&chars_read,
			NULL))
		{
			if (BYTES_TO_READ != chars_read)
			{
				done = true;
			}

			for (DWORD i = 0; i < chars_read; i++)
			{
				if ('A' == chars[i])
				{
					As++;
				}
			}
		}

		else
		{
			done = true;
		}

		//printf("%c", read_char);


	} while (!done);

	printf("%d\n", As);



	return 0;
}