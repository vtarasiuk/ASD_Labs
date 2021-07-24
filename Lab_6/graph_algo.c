#include "utils.h"
#include "graph_algo.h"


int** getOrientedDegrees(int** matrix, int vertices)
{
	int** degrees = mallocMatrix(vertices, 2);
	for (int i = 0; i < vertices; i++)
	{
		degrees[i][0] = 0; degrees[i][1] = 0;
	}

	for (int i = 0; i < vertices; i++)
	{
		for (int j = 0; j < vertices; j++)
		{
			if (matrix[i][j]) degrees[i][0]++;
			if (matrix[j][i]) degrees[i][1]++;
		}
	}

	return degrees;
}


int getOrientedRegularityDegree(int** degrees, int vertices)
{
	int degree = degrees[0];
	for (int i = 1; i < vertices; i++)
	{
		if (degree != degrees[i][0] || degree != degrees[i][1])
			return -1;
	}
	return degree;
}


int* getNonOrientedDegrees(int** matrix, int vertices)
{
	int* degrees = mallocArray(vertices);
	for (int i = 0; i < vertices; i++)
		degrees[i] = 0;

	for (int i = 0; i < vertices; i++)
	{
		for (int j = 0; j < vertices; j++)
		{
			if (matrix[i][j])
			{
				degrees[i]++;
				if (i == j) degrees[i]++;
			}
		}
	}

	return degrees;
}

int getNonOrientedRegularityDegree(int* degrees, int vertices)
{
	int degree = degrees[0];
	for (int i = 1; i < vertices; i++)
	{
		if (degree != degrees[i])
			return -1;
	}
	return degree;
}

int** getPathesOfLength2(int** matrix, int vertices, int* pathesAmount, int chainsOnly)
{
	int max_pathes = vertices * vertices * vertices;
	*pathesAmount = 0;
	int** accessOver1Matrix = multiplyMatrices(matrix, matrix, vertices);
	int** pathes = mallocMatrix(max_pathes, 3);

	for (int first = 0; first < vertices; first++)
	{
		for (int third = 0; third < vertices; third++)
		{
			if (accessOver1Matrix[first][third])
			{
				for (int second = 0; second < vertices; second++)
				{
					if (matrix[first][second] && matrix[second][third]
						&& (chainsOnly && !(first == second && second == third))
					)
					{
						pathes[*pathesAmount][0] = first;
						pathes[*pathesAmount][1] = second;
						pathes[*pathesAmount][2] = third;
						(*pathesAmount)++;
					}
				}
			}
		}
	}
	return pathes;
}

int** getPathesOfLength3(int** matrix, int vertices, int* pathesAmount, int chainsOnly)
{
	int max_pathes = vertices * vertices * vertices * vertices;
	*pathesAmount = 0;
	int** accessOver1Matrix = multiplyMatrices(matrix, matrix, vertices);
	int** accessOver2Matrix = multiplyMatrices(accessOver1Matrix, matrix, vertices);
	int** pathes = mallocMatrix(max_pathes, 4);

	for (int first = 0; first < vertices; first++)
	{
		for (int fourth = 0; fourth < vertices; fourth++)
		{
			if (accessOver2Matrix[first][fourth])
			{
				for (int second = 0; second < vertices; second++)
				{
					if (matrix[first][second] && accessOver1Matrix[second][fourth])
					{
						for (int third = 0; third < vertices; third++)
						{
							if (matrix[second][third] && matrix[third][fourth]
								&& (chainsOnly 
									&& !(first == second && second == third)
									&& !(second == third && third == fourth)
									)
								)
							{
								pathes[*pathesAmount][0] = first;
								pathes[*pathesAmount][1] = second;
								pathes[*pathesAmount][2] = third;
								pathes[*pathesAmount][3] = fourth;
								(*pathesAmount)++;
							}
						}
					}
				}
			}
		}
	}
	return pathes;
}


int** getAccessibilityMatrix(int** matrix, int vertices)
{
	int** accessMatrix = mallocI(vertices);
	int** currentPower = copyMatrix(matrix, vertices, vertices);
	for (int p = 0; p < vertices - 1; p++)
	{
		for (int i = 0; i < vertices; i++)
		{
			for (int j = 0; j < vertices; j++)
			{
				accessMatrix[i][j] = accessMatrix[i][j] | currentPower[i][j];
			}
		}
		currentPower = multiplyMatrices(currentPower, matrix, vertices);
	}
	return accessMatrix;
}

int** getStrongConnectionMatrix(int** matrix, int vertices)
{
	int** transponed = transponateMatrix(matrix, vertices);
	int** res = mallocMatrix(vertices, vertices);
	for (int i = 0; i < vertices; i++)
		for (int j = 0; j < vertices; j++)
			res[i][j] = matrix[i][j] && transponed[i][j];
	return res;
}

int** getStrongConnectionComponents(int** strongConnectionMatrix, int vertices, int* componentsAmount)
{
	int** components = mallocMatrix(vertices + 1, vertices + 1);
	for (int i = 0; i < vertices + 1; i++)
		for (int j = 0; j < vertices + 1; j++)
			components[i][j] = -1;

	int* banned = mallocArray(vertices);
	int bannedAmount = 0;
	*componentsAmount = 0;
	int ix;
	while (bannedAmount < vertices)
	{
		int firstNotBanned = -1;
		int isBanned;
		for (int i = 0; i < vertices; i++)
		{
			isBanned = 0;
			for (int j = 0; j < bannedAmount; j++)
			{
				if (banned[j] == i)
				{
					isBanned = 1;
					break;
				}
			}
			if (!isBanned)
			{
				firstNotBanned = i;
				break;
			}
		}
		ix = 0;
		for (int i = 0; i < vertices; i++)
		{
			if (strongConnectionMatrix[firstNotBanned][i])
			{
				components[*componentsAmount][ix] = i;
				ix++;
				banned[bannedAmount] = i;
				bannedAmount++;
			}
		}
		(*componentsAmount)++;
	}
	return components;
}

int** getCondensedMatrix(int** strongConnectionComponents, int componentsAmount, int** accessMatrix, int vertices)
{
	int** condensedMatrix = mallocMatrix(componentsAmount, componentsAmount);
	for (int i = 0; i < componentsAmount; i++)
		for (int j = 0; j < componentsAmount; j++)
			condensedMatrix[i][j] = 0;
			

	int* colors = mallocArray(vertices);
	int ix;
	for (int i = 0; i < componentsAmount; i++)
	{
		ix = 0;
		for (int j = 0; ix != -1; j++)
		{
			ix = strongConnectionComponents[i][j];
			if (ix != -1)
				colors[ix] = i;
		}
	}
	for (int i = 0; i < vertices; i++)
		for (int j = 0; j < vertices; j++)
			if (accessMatrix[i][j] && colors[i] != colors[j])
				condensedMatrix[colors[i]][colors[j]] = 1;
	return condensedMatrix;
}


int** multiplyMatrices(int** m1, int** m2, int vertices)
{
	int** res = mallocMatrix(vertices, vertices);
	for (int i = 0; i < vertices; i++)
		for (int j = 0; j < vertices; j++)
		{
			res[i][j] = 0;
			for (int k = 0; k < vertices; k++)
				res[i][j] = res[i][j] || m1[i][k] && m2[k][j];
		}
	return res;
}

int** transponateMatrix(int** m, int vertices)
{
	int** res = mallocMatrix(vertices, vertices);
	for (int i = 0; i < vertices; i++)
		for (int j = 0; j < vertices; j++)
			res[i][j] = m[j][i];
	return res;
}
