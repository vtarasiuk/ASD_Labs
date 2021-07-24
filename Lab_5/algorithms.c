#include<windows.h>
#include <conio.h>;
#include "drawing.h";
#include "algorithms.h";
#include "memory.h";

int** dfs(HWND* hWnd, Vertex** V, int** M, int size)
{
	pause(hWnd);
	int** tree = get2DArray(size, size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			tree[i][j] = 0;
	int* used = getArray(size);
	for (int i = 0; i < size; i++)
		used[i] = 0;
	int unused;
	while (1)
	{
		unused = -1;
		for (int i = 0; i < size; i++)
			if (used[i] == 0)
			{
				unused = i;
				break;
			}
		if (unused == -1)
			break;

		_dfs(hWnd, V, M, used, size, tree, unused);
	}
	return tree;
}

void _dfs(HWND* hWnd,
	Vertex** V,
	int** M,
	int* used,
	int size,
	int** traverse,
	int index)
{
	V[index]->title = index + 1;
	used[index] = 1;
	pause(hWnd);
	for (int i = 0; i < size; i++)
	{
		if (M[index][i] && !used[i])
		{
			traverse[index][i] = 1;
			_dfs(hWnd, V, M, used, size, traverse, i);
		}
	}
}

int** bfs (HWND* hWnd, Vertex** V, int** M, int size)
{
	pause(hWnd);
	int** tree = get2DArray(size, size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			tree[i][j] = 0;
	int* used = getArray(size);
	for (int i = 0; i < size; i++)
		used[i] = 0;
	int unused;
	while (1)
	{
		unused = -1;
		for (int i = 0; i < size; i++)
			if (used[i] == 0)
			{
				unused = i;
				break;
			}
		if (unused == -1)
			break;

		_bfs(hWnd, V, M, used, size, tree, unused);
	}
	return tree;
}
void _bfs(HWND* hWnd,
	Vertex** V,
	int** M,
	int* used,
	int size,
	int** traverse,
	int index)
{
	int* queue = getArray(size);
	for (int i = 0; i < size; i++)
		queue[i] = -1;
	int queuePos = 0;
	queue[queuePos] = index;
	used[index] = 1;

	int current;
	int ix = 0;
	while (ix < size && queue[ix] != -1)
	{
		current = queue[ix]; ix++;
		V[current]->title = current + 1;
		pause(hWnd);
		for (int i = 0; i < size; i++)
		{
			if (M[current][i] && !used[i])
			{
				traverse[current][i] = 1;
				queuePos++;
				queue[queuePos] = i;
				used[i] = 1;
			}
		}
	}

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
