#include <stdlib.h>
#include <math.h>

#include "utils.h";

double getK(int seed)
{
	return 1.0
		- ((seed / 10) % 10) * 0.01
		- (seed % 10) * 0.005
		- 0.05;
}


int** getM(int size, int seed, double k)
{
	srand(seed);
	int** matrix = createMatrix(size, size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			matrix[i][j] = (double)k * ((double)rand() / (double)RAND_MAX * 2) > 1.0 ? 1 : 0;
			matrix[i][j] = round(matrix[i][j] * ((double)rand() / (double)RAND_MAX * 2) * 100);
		}
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			int c = matrix[i][j] != matrix[j][i];
			int d = matrix[i][j] == matrix[j][i] && matrix[i][j] == 1;
			if (j > i)
				d *= (matrix[i][j] > 0);
			matrix[i][j] *= c + d;
		}
	return matrix;
}

int** symmetrify(int** M, int size)
{
	int** matrix = createMatrix(size, size);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			matrix[i][j] = matrix[j][i] = M[i][j];
		}
	}
	return matrix;
}
