#include <limits.h>
#include <windows.h>

#include "graph.h"
#include "utils.h"

int** execute_prim_algo(HWND hWnd, 
	Graph** g, 
	int** M, 
	int size, 
	int vertexRadius, 
	int padding, 
	Point* center)
{
	int** tree = createMatrix(size, size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			tree[i][j] = 0;
	int* included = createArray(size);
	for (int i = 0; i < size; i++)
		included[i] = 0;
	included[0] = 1;

	(*g) = makeGraph(size, vertexRadius, padding, tree, center);

	int minFrom, minTo, minWeight;
	for (int p = 0; p < size - 1; p++)
	{
		minFrom = -1;
		minTo = -1;
		minWeight = INT_MAX;
		for (int i = 0; i < size; i++)
		{
			if (included[i])
				for (int j = 0; j < size; j++)
				{
					if (i != j
						&& M[i][j]
						&& !included[j]
						&& M[i][j] < minWeight
						)
					{
						minFrom = i;
						minTo = j;
						minWeight = M[i][j];
					}
				}
		}
		if (minFrom != -1 && minTo != -1)
		{
			pause(hWnd);
			tree[minFrom][minTo] = minWeight;
			tree[minTo][minFrom] = minWeight;
			(*g) = makeGraph(size, vertexRadius, padding, tree, center);
			included[minTo] = 1;
		}
	}
	return tree;
}
