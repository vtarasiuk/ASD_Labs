#include<windows.h>
#include <conio.h>;
#include "graph.h";
#include "utils.h";
#include "graph_traversal.h";

int** traverseGraph(int algo, HWND* hWnd, Vertex** vertices, int** matrix, int verticesAmount)
{
	pause(hWnd);
	int** traverseTree = mallocMatrix(verticesAmount, verticesAmount);
	for (int i = 0; i < verticesAmount; i++)
		for (int j = 0; j < verticesAmount; j++)
			traverseTree[i][j] = 0;
	int* visited = mallocArray(verticesAmount);
	for (int i = 0; i < verticesAmount; i++)
		visited[i] = 0;
	int unvisited;
	while (1)
	{
		unvisited = -1;
		for (int i = 0; i < verticesAmount; i++)
			if (visited[i] == 0)
			{
				unvisited = i;
				break;
			}
		if (unvisited == -1)
			break;

		if (algo == 1)
			dfs(hWnd, vertices, matrix, visited, verticesAmount, traverseTree, unvisited);
		else if (algo == 2)
			bfs(hWnd, vertices, matrix, visited, verticesAmount, traverseTree, unvisited);
}
	return traverseTree;
}

void dfs(HWND* hWnd,
	Vertex** vertices, 
	int** matrix,
	int* visited, 
	int verticesAmount, 
	int** traverseTree, 
	int calledFor)
{
	vertices[calledFor]->title = calledFor + 1;
	visited[calledFor] = 1;
	pause(hWnd);
	for (int i = 0; i < verticesAmount; i++)
	{
		if (matrix[calledFor][i] && !visited[i])
		{
			traverseTree[calledFor][i] = 1;
			dfs(hWnd, vertices, matrix, visited, verticesAmount, traverseTree, i);
		}
	}
}

void bfs(HWND* hWnd,
	Vertex** vertices,
	int** matrix,
	int* visited,
	int verticesAmount,
	int** traverseTree,
	int calledFor)
{
	int* queue = mallocArray(verticesAmount);
	for (int i = 0; i < verticesAmount; i++)
		queue[i] = -1;
	int queuePos = 0;
	queue[queuePos] = calledFor;
	visited[calledFor] = 1;

	int current;
	int ix = 0;
	while (ix < verticesAmount && queue[ix] != -1)
	{
		current = queue[ix]; ix++;
		vertices[current]->title = current + 1;
		pause(hWnd);
		for (int i = 0; i < verticesAmount; i++)
		{
			if (matrix[current][i] && !visited[i])
			{
				traverseTree[current][i] = 1;
				queuePos++;
				queue[queuePos] = i;
				visited[i] = 1;
			}
		}
	}

}

