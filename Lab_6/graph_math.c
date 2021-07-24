#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#include "graph_math.h"

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

Point** _getPolygonVertices(int n, double radius, Point* center)
{
	double angle = 2 * M_PI / n;
	Point** vertices = (Point**)malloc(n * sizeof(Point*));
	for (int i = 0; i < n; i++)
	{
		float x = center->x + radius * sin((double)i * angle);
		float y = center->y + radius * cos((double)i * angle);
		vertices[i] = point(x, y);
	}
	return vertices;
}

float _getPolygonRadius(int n, double side)
{
	return side / (2 * sin(M_PI / n));
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
	return _getPolygonVertices(circulesAmount, 
		_getPolygonRadius(circulesAmount, (radius + padding) * 2), 
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
