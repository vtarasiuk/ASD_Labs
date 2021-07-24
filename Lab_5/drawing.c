#define _USE_MATH_DEFINES
#include <windows.h>
#include <math.h>
#include <stdlib.h>

#include "drawing.h"
#include "memory.h"

double k(int seed)
{
	return 1.0
		- ((seed / 10) % 10) * 0.01
		- (seed % 10) * 0.005
		- 0.15;
}

int** createMatrix(int size, int seed, double k)
{
	srand(seed);
	int** matrix = get2DArray(size, size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			matrix[i][j] = (double)k * ((double)rand() / (double)RAND_MAX * 2) > 1.0 ? 1 : 0;
	return matrix;
}

int** makeSimmetric(int** M, int size)
{
	int** matrix = get2DArray(size, size);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			matrix[i][j] = matrix[j][i] = M[i][j];
		}
	}
	return matrix;
}

Graph* generateGraph(
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
		vertex->title = 0;
		vertex->pos = point(positions[i]->x, positions[i]->y);
		graph->vertices[i] = vertex;
	}
	return graph;
}


void drawCircle(HDC hdc, HPEN pen,
	Point* p, double radius)
{
	SelectObject(hdc, pen);
	Box* rect = getCircumscribedRectangle(p, radius);
	Ellipse(hdc, rect->left, rect->top, rect->right, rect->bot);
}

void drawVertex(HDC hdc, HPEN pen,
	Vertex* v, double radius)
{
	drawCircle(hdc, pen, v->pos, radius);
	char buf[30];
	_itoa_s(v->title, buf, 30, 10);
	int length = strlen(buf);
	int xOffset = 4 * length;
	TextOutA(hdc, v->pos->x - xOffset, v->pos->y - 7, buf, length);
}


void drawLine(HDC hdc, HPEN pen,
	Point* from, Point* to)
{
	SelectObject(hdc, pen);
	MoveToEx(hdc, from->x, from->y, NULL);
	LineTo(hdc, to->x, to->y);
}

void drawPath(HDC hdc, HPEN pen,
	Path* path)
{
	SelectObject(hdc, pen);
	MoveToEx(hdc, path->points[0]->x, path->points[0]->y, NULL);
	for (int i = 1; i < path->length; i++)
	{
		LineTo(hdc, path->points[i]->x, path->points[i]->y);
	}
}

void drawArrow(HDC hdc, HPEN pen,
	Point* p, double angleInRadians)
{
	Path* path = getArrowPath(p, angleInRadians, 15, 0.3);
	drawPath(hdc, pen, path);
}

void drawLineArrow(HDC hdc, HPEN pen,
	Point* from, Point* to)
{
	drawLine(hdc, pen, from, to);
	double angle = getLineAngle(from, to);
	drawArrow(hdc, pen, to, angle);
}


void directConnectVertices(HDC hdc, HPEN pen,
	Vertex* from, Vertex* to,
	int radius,
	int withArrow)
{
	Point* fromPoint = getPointOnLineOnDistance(from->pos, to->pos, radius);
	Point* toPoint = getPointOnLineOnDistance(to->pos, from->pos, radius);
	if (withArrow) drawLineArrow(hdc, pen, fromPoint, toPoint);
	else drawLine(hdc, pen, fromPoint, toPoint);
}

void offsetConnectVertices(HDC hdc, HPEN pen,
	Vertex* from, Vertex* to,
	int radius,
	int offset,
	double angleOffset,
	int withArrow)
{
	Point* fromPoint = rotatePoint(
		getPointOnLineOnDistance(from->pos, to->pos, radius),
		from->pos, angleOffset);
	Point* toPoint = rotatePoint(
		getPointOnLineOnDistance(to->pos, from->pos, radius),
		to->pos, -angleOffset);
	Point* middlePoint = getMiddleOffsetByNormal(fromPoint, toPoint, offset);
	drawLine(hdc, pen, fromPoint, middlePoint);
	if (withArrow) drawLineArrow(hdc, pen, middlePoint, toPoint);
	else drawLine(hdc, pen, middlePoint, toPoint);
}

void makeLoop(HDC hdc, HPEN pen,
	Vertex* v,
	Point* graphCenter,
	int radius,
	int offset,
	double angleDifference,
	int withArrow)
{
	Point* fromPoint = rotatePoint(
		getPointOnLineOnDistance(v->pos, graphCenter, -radius),
		v->pos, angleDifference);
	Point* toPoint = rotatePoint(
		getPointOnLineOnDistance(v->pos, graphCenter, -radius),
		v->pos, -angleDifference);
	Point* middlePoint = getPointOnLineOnDistance(v->pos, graphCenter, -offset);
	drawLine(hdc, pen, fromPoint, middlePoint);
	if (withArrow) drawLineArrow(hdc, pen, middlePoint, toPoint);
	else drawLine(hdc, pen, middlePoint, toPoint);
}

void drawGraph(HDC hdc, HPEN pen, HPEN highlightPen,
	Graph* graph,
	int oriented)
{
	for (int i = 0; i < graph->power; i++)
	{
		if (highlightPen != NULL && graph->vertices[i]->title != 0)
			drawVertex(hdc, highlightPen, graph->vertices[i], graph->vertexRadius);
		else
			drawVertex(hdc, pen, graph->vertices[i], graph->vertexRadius);
	}
	double offset;
	int needOffset;
	for (int i = 0; i < graph->power; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			int dist = abs(i - j);
			if (dist > graph->power / 2)
			{
				dist = graph->power / 2 - (dist - graph->power / 2);
			}
			int sideLength = graph->power / 3 + 1;
			int c1 = 0, c2 = sideLength - 1, c3 = (sideLength - 1) * 2;
			int sameLine = (i >= c1 && i <= c2 && j >= c1 && j <= c2)
				|| (i >= c2 && i <= c3 && j >= c2 && j <= c3)
				|| ((i >= c3 || i == 0) && (j >= c3 || j == 0));
			if (sameLine && dist > 1)
			{
				offset = graph->vertexRadius * dist + dist;
				needOffset = 1;
			}
			else
			{
				offset = graph->vertexRadius / 2;
				needOffset = 0;
			}

			if (i == j && graph->matrix[i][j])
			{
				makeLoop(hdc, pen,
					graph->vertices[i],
					graph->center,
					graph->vertexRadius,
					graph->vertexRadius * 3,
					0.5,
					oriented);
			}
			else if (graph->matrix[i][j] && graph->matrix[j][i] && oriented)
			{
				offsetConnectVertices(hdc, pen,
					graph->vertices[i],
					graph->vertices[j],
					graph->vertexRadius,
					(dist == 5) ? offset * 9.5 : offset,
					0.1,
					1);
				offsetConnectVertices(hdc, pen,
					graph->vertices[j],
					graph->vertices[i],
					graph->vertexRadius,
					(dist == 5) ? offset * 9.5 : offset,
					0.1,
					1);
			}
			else if (graph->matrix[i][j])
			{
				if (!needOffset)
					directConnectVertices(hdc, pen,
						graph->vertices[i],
						graph->vertices[j],
						graph->vertexRadius,
						oriented);
				else
					offsetConnectVertices(hdc, pen,
						graph->vertices[i],
						graph->vertices[j],
						graph->vertexRadius,
						offset,
						0.1,
						oriented);
			}
			else if (graph->matrix[j][i])
			{
				if (!needOffset)
					directConnectVertices(hdc, pen,
						graph->vertices[j],
						graph->vertices[i],
						graph->vertexRadius,
						oriented);
				else
					offsetConnectVertices(hdc, pen,
						graph->vertices[j],
						graph->vertices[i],
						graph->vertexRadius,
						offset,
						0.1,
						oriented);
			}
		}
	}
}


Point* point(double x, double y)
{
	Point* p = (Point*)malloc(sizeof(Point));
	p->x = x; p->y = y;
	return p;
}

double _degreesToRadians(double angleInDegrees)
{
	return angleInDegrees * M_PI / 180.0;
}

Point** _getLineVertices(int n, Point* from, Point* to)
{
	Point** line = (Point**)malloc(n * sizeof(Point*));
	double xStep = (to->x - from->x) / ((double)n);
	double yStep = (to->y - from->y) / ((double)n);
	line[0] = from;
	for (int i = 1; i < n; i++)
		line[i] = point(line[i - 1]->x + xStep, line[i - 1]->y + yStep);
	return line;
}

void _rotateLine(Point** line, int n, Point* pivot, double angle)
{
	for (int i = 0; i < n; i++)
		line[i] = rotatePoint(line[i], pivot, angle);
}

Point** _getTriangleVertices(int n, double side, Point* center)
{
	double circumcircleRadius = side / sqrt(3);
	double incircleRadius = side / (2 * sqrt(3));
	int rest = n % 3;
	int l1 = n / 3 + (rest == 2 || rest == 1);
	int l2 = n / 3 + (rest == 2);
	int l3 = n / 3;

	Point** line1 = _getLineVertices(l1,
		point(center->x - side / 2, center->y + incircleRadius),
		point(center->x + side / 2, center->y + incircleRadius));
	Point** line2 = _getLineVertices(l2,
		point(center->x - side / 2, center->y + incircleRadius),
		point(center->x + side / 2, center->y + incircleRadius));
	_rotateLine(line2, l2, center, -_degreesToRadians(120));
	Point** line3 = _getLineVertices(l3,
		point(center->x - side / 2, center->y + incircleRadius),
		point(center->x + side / 2, center->y + incircleRadius));
	_rotateLine(line3, l3, center, -_degreesToRadians(240));

	Point** vertices = (Point**)malloc(n * sizeof(Point*));
	int ix = 0;
	for (int i = 0; i < l1; i++, ix++)
		vertices[ix] = line1[i];
	for (int i = 0; i < l2; i++, ix++)
		vertices[ix] = line2[i];
	for (int i = 0; i < l3; i++, ix++)
		vertices[ix] = line3[i];
	return vertices;
}

double getLineAngle(Point* p1, Point* p2)
{
	double degrees;
	if (p2->x - p1->x == 0)
	{
		if (p2->y > p1->y)
			degrees = 90;
		else
			degrees = 270;
	}
	else
	{
		double riseoverrun = (double)(p2->y - p1->y) / (double)(p2->x - p1->x);
		double radians = atan(riseoverrun);
		degrees = radians * ((double)180 / M_PI);

		if ((p2->x - p1->x) < 0 || (p2->y - p1->y) < 0)
			degrees += 180;
		if ((p2->x - p1->x) > 0 && (p2->y - p1->y) < 0)
			degrees -= 180;
		if (degrees < 0)
			degrees += 360;
	}
	return M_PI + _degreesToRadians(degrees);
}

Point** getCirculesPositions(Point* center, int circulesAmount, double radius, double padding)
{
	return _getTriangleVertices(circulesAmount,
		(circulesAmount / 3 + 2) * (radius + padding),
		center);
}

Point* getPointOnLineOnDistance(Point* p1, Point* p2, double distance)
{
	Point* vector = point(p2->x - p1->x, p2->y - p1->y);
	double length = sqrt(vector->x * vector->x + vector->y * vector->y);
	double normalX = (double)vector->x / length;
	double normalY = (double)vector->y / length;
	return point((double)p1->x + normalX * distance, (double)p1->y + normalY * distance);
}

Path* getArrowPath(Point* p, double angleInRadians, double length, double width)
{
	Path* path = (Path*)malloc(sizeof(Path));
	path->length = 3;
	path->points = malloc(3 * sizeof(Point*));
	//angleInRadians = M_PI - angleInRadians;
	path->points[0] = point(
		p->x + length * cos(angleInRadians + width),
		p->y + length * sin(angleInRadians + width));
	path->points[1] = p;
	path->points[2] = point(
		p->x + length * cos(angleInRadians - width),
		p->y + length * sin(angleInRadians - width));
	return path;
}

Box* getCircumscribedRectangle(Point* center, double radius)
{
	Box* rect = (Box*)malloc(sizeof(Box));
	rect->left = center->x - radius;
	rect->top = center->y - radius;
	rect->right = center->x + radius;
	rect->bot = center->y + radius;
	return rect;
}

int isPointInCircule(Point* p, Point* center, double radius)
{
	double dX = (double)p->x - (double)center->x;
	double dY = (double)p->y - (double)center->y;
	double distanceSquared = dX * dX + dY * dY;
	if (distanceSquared <= radius * radius) return 1;
	return 0;
}

int collideLineMiddleAndCircule(Point* p1, Point* p2, Point* center, double radius)
{
	double middleX = ((double)p1->x + (double)p2->x) / 2;
	double middleY = ((double)p1->y + (double)p2->y) / 2;
	double dX = middleX - (double)center->x;
	double dY = middleY - (double)center->y;
	double distanceSquared = dX * dX + dY * dY;
	if (distanceSquared <= radius * radius) return 1;
	return 0;
}

Point* getMiddleOffsetByNormal(Point* p1, Point* p2, double offset)
{
	double middleX = ((double)p1->x + (double)p2->x) / 2;
	double middleY = ((double)p1->y + (double)p2->y) / 2;
	double dX = p2->x - p1->x;
	double dY = p2->y - p1->y;
	double distance = sqrt(dX * dX + dY * dY);
	dX /= distance; dY /= distance;
	double normalX = -dY;
	double normalY = dX;
	return point(middleX + normalX * offset, middleY + normalY * offset);
}

Point* rotatePoint(Point* original, Point* center, double angle)
{
	double s = sin(angle);
	double c = cos(angle);

	Point* p = point(original->x, original->y);

	p->x -= center->x;
	p->y -= center->y;

	double newX = p->x * c - p->y * s;
	double newY = p->x * s + p->y * c;

	p->x = newX + center->x;
	p->y = newY + center->y;
	return p;
}
