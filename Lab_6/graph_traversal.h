#pragma once
#include "windows.h";
#include "graph.h";

int** traverseGraph(int algo, HWND* hWnd, Vertex** vertices, int** matrix, int verticesAmount);
void dfs(HWND* hWnd,
	Vertex** vertices,
	int** matrix,
	int* visited,
	int verticesAmount,
	int** traverseTree,
	int calledFor);
void bfs(HWND* hWnd,
	Vertex** vertices,
	int** matrix,
	int* visited,
	int verticesAmount,
	int** traverseTree,
	int calledFor);
