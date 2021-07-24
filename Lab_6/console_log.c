#include <Windows.h>
#include <stdio.h>

void initializeLogConsole()
{
	if (AllocConsole()) {
		FILE* pCout;
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		SetConsoleTitle(L"Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
	system("cls");
}

void printMatrix(int** matrix, int vertices)
{
	printf("#\t");
	for (int i = 0; i < vertices; i++)
	{
		printf("%i\t", i + 1);
	}
	printf("\n\n");
	for (int i = 0; i < vertices; i++)
	{
		printf("%i\t", i + 1);
		for (int j = 0; j < vertices; j++)
		{
			printf("%i\t", matrix[i][j]);
		}
		printf("\n");
	}
}

void printLine(int length)
{
	printf("\n\n");
	for (int i = 0; i < length; i++)
		printf("-");
	printf("\n\n");
}
