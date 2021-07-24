#include<windows.h>

int** createMatrix(int n, int m)
{
	int** matrix = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++)
		matrix[i] = (int*)malloc(m * sizeof(int));
	return matrix;
}

int** createI(int n)
{
	int** I = createMatrix(n, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			I[i][j] = 0;
	for (int i = 0; i < n; i++)
		I[i][i] = 1;
	return I;
}

int** copyMatrix(int** M, int n, int m)
{
	int** matrix = createMatrix(n, m);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			matrix[i][j] = M[i][j];
	return matrix;
}

int* createArray(int n)
{
	int* arr = (int*)malloc(n * sizeof(int));
	return arr;
}

void pause(HWND* hWnd)
{
	while (1) {
		if (_kbhit()) {
			char c = _getch();
			InvalidateRect(*hWnd, NULL, TRUE);
			break;
		}
	}
}
