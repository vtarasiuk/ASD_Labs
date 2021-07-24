#pragma once
#include "graph_math.h"

typedef struct Vertex
{
	int title;
	struct Point* pos;
} Vertex;

typedef struct Graph
{
	struct Vertex** vertices;
	int power;
	int vertexRadius;
	int** matrix;
	Point* center;
} Graph;

Graph* makeGraph(
	int verticesAmount,
	int vertexRadius,
	int padding,
	int** adjacencyMatrix,
	Point* center);
