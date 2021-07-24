#include <stdlib.h>

#include "graph.h"
#include "graph_math.h"

Graph* makeGraph(
	int size,
	int radius,
	int padding,
	int** M,
	Point* c)
{
	Graph* graph = malloc(sizeof(Graph));
	graph->matrix = M;
	graph->vertexRadius = radius;
	graph->power = size;
	graph->center = c;
	Point** positions = getCirculesPositions(c, size, radius, padding);
	graph->vertices = (Vertex*)malloc(size * sizeof(Vertex));
	for (int i = 0; i < size; i++)
	{
		Vertex* vertex = (Vertex*)malloc(sizeof(Vertex));
		vertex->title = i + 1;
		vertex->pos = point(positions[i]->x, positions[i]->y);
		graph->vertices[i] = vertex;
	}
	return graph;
}

