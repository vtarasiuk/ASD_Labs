#include <stdlib.h>

#include "memory.h";

int** get2DArray(int n, int m)
{
	int** matrix = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++)
		matrix[i] = (int*)malloc(m * sizeof(int));
	return matrix;
}

int** mallocI(int n)
{
	int** I = get2DArray(n, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			I[i][j] = 0;
	for (int i = 0; i < n; i++)
		I[i][i] = 1;
	return I;
}

int** copyMatrix(int** base, int n, int m)
{
	int** matrix = get2DArray(n, m);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			matrix[i][j] = base[i][j];
	return matrix;
}

int* getArray(int n)
{
	int* arr = (int*)malloc(n * sizeof(int));
	return arr;
}
