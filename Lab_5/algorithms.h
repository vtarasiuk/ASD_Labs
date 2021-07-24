#pragma once
#include "windows.h";
#include "drawing.h";

int** dfs(HWND* hWnd, Vertex** vertices, int** matrix, int verticesAmount);
void _dfs(HWND* hWnd,
	Vertex** vertices,
	int** matrix,
	int* visited,
	int verticesAmount,
	int** traverseTree,
	int calledFor);
int** bfs(HWND* hWnd, Vertex** vertices, int** matrix, int verticesAmount);
void _bfs(HWND* hWnd,
	Vertex** vertices,
	int** matrix,
	int* visited,
	int verticesAmount,
	int** traverseTree,
	int calledFor);
void pause(HWND* hWnd);
