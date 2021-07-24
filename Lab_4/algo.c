#include "memory.h"
#include "algo.h"


int** or_degrees(int** M, int size)
{
	int** res = getMMemory(size, 2);
	for (int i = 0; i < size; i++)
	{
		res[i][0] = 0; res[i][1] = 0;
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (M[i][j]) res[i][0]++;
			if (M[j][i]) res[i][1]++;
		}
	}

	return res;
}

int** multiply(int** M1, int** M2, int size)
{
	int** res = getMMemory(size, size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			res[i][j] = 0;
			for (int k = 0; k < size; k++)
				res[i][j] = res[i][j] || M1[i][k] && M2[k][j];
		}
	return res;
}

int or_reg_degrees(int** degrees, int size)
{
	int degree = degrees[0];
	for (int i = 1; i < size; i++)
	{
		if (degree != degrees[i][0] || degree != degrees[i][1])
			return -1;
	}
	return degree;
}


int* nor_degrees(int** M, int size)
{
	int* degrees = mallocArray(size);
	for (int i = 0; i < size; i++)
		degrees[i] = 0;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (M[i][j])
			{
				degrees[i]++;
				if (i == j) degrees[i]++;
			}
		}
	}

	return degrees;
}

int nor_reg_degree(int* degrees, int size)
{
	int degree = degrees[0];
	for (int i = 1; i < size; i++)
	{
		if (degree != degrees[i])
			return -1;
	}
	return degree;
}

int** pathes_2(int** M, int size, int* pathes_amount)
{
	int max_pathes = size * size * size;
	*pathes_amount = 0;
	int** accessOver1Matrix = multiply(M, M, size);
	int** pathes = getMMemory(max_pathes, 3);

	for (int first = 0; first < size; first++)
	{
		for (int third = 0; third < size; third++)
		{
			if (accessOver1Matrix[first][third])
			{
				for (int second = 0; second < size; second++)
				{
					if (M[first][second] && M[second][third]
						&& (!(first == second && second == third))
						)
					{
						pathes[*pathes_amount][0] = first;
						pathes[*pathes_amount][1] = second;
						pathes[*pathes_amount][2] = third;
						(*pathes_amount)++;
					}
				}
			}
		}
	}
	return pathes;
}



int** access_M(int** M, int size)
{
	int** accessMatrix = mallocI(size);
	int** currentPower = copyMatrix(M, size, size);
	for (int p = 0; p < size - 1; p++)
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				accessMatrix[i][j] = accessMatrix[i][j] | currentPower[i][j];
			}
		}
		currentPower = multiply(currentPower, M, size);
	}
	return accessMatrix;
}

int** strong_connected_M(int** M, int size)
{
	int** transponed = transpone(M, size);
	int** res = getMMemory(size, size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			res[i][j] = M[i][j] && transponed[i][j];
	return res;
}

int** strong_components(int** strong_M, int size, int* components_size)
{
	int** res = getMMemory(size + 1, size + 1);
	for (int i = 0; i < size + 1; i++)
		for (int j = 0; j < size + 1; j++)
			res[i][j] = -1;

	int* used = mallocArray(size);
	int used_size = 0;
	*components_size = 0;
	int index;
	while (used_size < size)
	{
		int firstNotUsed = -1;
		int isUsed;
		for (int i = 0; i < size; i++)
		{
			isUsed = 0;
			for (int j = 0; j < used_size; j++)
			{
				if (used[j] == i)
				{
					isUsed = 1;
					break;
				}
			}
			if (!isUsed)
			{
				firstNotUsed = i;
				break;
			}
		}
		index = 0;
		for (int i = 0; i < size; i++)
		{
			if (strong_M[firstNotUsed][i])
			{
				res[*components_size][index] = i;
				index++;
				used[used_size] = i;
				used_size++;
			}
		}
		(*components_size)++;
	}
	return res;
}

int** pathes_3(int** M, int size, int* pathes_amount)
{
	int max_pathes = size * size * size * size;
	*pathes_amount = 0;
	int** accessOver1Matrix = multiply(M, M, size);
	int** accessOver2Matrix = multiply(accessOver1Matrix, M, size);
	int** pathes = getMMemory(max_pathes, 4);

	for (int first = 0; first < size; first++)
	{
		for (int fourth = 0; fourth < size; fourth++)
		{
			if (accessOver2Matrix[first][fourth])
			{
				for (int second = 0; second < size; second++)
				{
					if (M[first][second] && accessOver1Matrix[second][fourth])
					{
						for (int third = 0; third < size; third++)
						{
							if (M[second][third] && M[third][fourth]
								&& (!(first == second && second == third)
									&& !(second == third && third == fourth)
									)
								)
							{
								pathes[*pathes_amount][0] = first;
								pathes[*pathes_amount][1] = second;
								pathes[*pathes_amount][2] = third;
								pathes[*pathes_amount][3] = fourth;
								(*pathes_amount)++;
							}
						}
					}
				}
			}
		}
	}
	return pathes;
}

int** condensed_M(int** strong_components, int components_size, int** access_M, int size)
{
	int** res = getMMemory(components_size, components_size);
	for (int i = 0; i < components_size; i++)
		for (int j = 0; j < components_size; j++)
			if (i == j) res[i][j] = 1;
			else res[i][j] = 0;


	int* colors = mallocArray(size);
	int ix;
	for (int i = 0; i < components_size; i++)
	{
		ix = 0;
		for (int j = 0; ix != -1; j++)
		{
			ix = strong_components[i][j];
			if (ix != -1)
				colors[ix] = i;
		}
	}
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (access_M[i][j] && colors[i] != colors[j])
				res[colors[i]][colors[j]] = 1;
	return res;
}

int** transpone(int** M, int size)
{
	int** res = getMMemory(size, size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			res[i][j] = M[j][i];
	return res;
}
