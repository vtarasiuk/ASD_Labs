#pragma once
#pragma once
#include <windows.h>

typedef struct Point
{
	int x;
	int y;
} Point;

typedef struct Path
{
	struct Point** points;
	int length;
} Path;

typedef struct Box
{
	int left;
	int right;
	int bot;
	int top;
} Box;

typedef struct Vertex
{
	int title;
	struct Point* pos;
} Vertex;

typedef struct Graph
{
	struct Vertex** vertices;
	int strength;
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
void drawGraph(HDC hdc, HPEN pen,
	Graph* graph,
	int oriented);

int** randomizeM(int vertices, int seed);
int** makeSymmetricM(int** original, int vertices);

Point* point(double x, double y);
Point** getVPos(Point* center, int circulesAmount, double radius, double padding);
Box* getRect(Point* center, double radius);
Point* getPointOnLineOnDistance(Point* p1, Point* p2, double distance);
Path* getArrowPath(Point* p, double angleInRadians, double length, double width);
double getLineAngle(Point* p1, Point* p2);
Point* getMiddleOffsetByNormal(Point* p1, Point* p2, double offset);
int isLineInCircule(Point* p1, Point* p2, Point* center, double radius);
Point* rotatePoint(Point* original, Point* center, double angle);
int isPointInCircule(Point* p, Point* center, double radius);
