#pragma once

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

Point* point(double x, double y);
Point** getCirculesPositions(Point* center, int circulesAmount, double radius, double padding);
Box* getCircumscribedRectangle(Point* center, double radius);
Point* getPointOnLineOnDistance(Point* p1, Point* p2, double distance);
Path* getArrowPath(Point* p, double angleInRadians, double length, double width);
double getLineAngle(Point* p1, Point* p2);
Point* getMiddleOffsetByNormal(Point* p1, Point* p2, double offset);
int collideLineMiddleAndCircule(Point* p1, Point* p2, Point* center, double radius);
Point* rotatePoint(Point* original, Point* center, double angle);
int isPointInCircule(Point* p, Point* center, double radius);
