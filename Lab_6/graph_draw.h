#pragma once
#include<windows.h>
#include<math.h>

#include "graph.h"
#include "graph_math.h"

void drawGraph(HDC hdc, HPEN pen, HPEN highlightPen,
	Graph* graph,
	int oriented);
