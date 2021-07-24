#include<windows.h>
#include<stdio.h>
#include<math.h>

#include "graph.h"
#include "graph_draw.h"
#include "graph_spanning_tree.h"
#include "random.h"
#include "console_log.h"
#include "utils.h"

typedef struct {
	HWND* hWnd;
	int windowWidth;
	int windowHeight;
} ScriptParameters;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI executeReportScript(LPVOID lpParam);
void drawGraphs(HDC* hdc);

char* PROGRAM_TITLE = "LAB 6";
const int WINDOW_WIDTH = 1500;
const int WINDOW_HEIGHT = 800;
const int VERTICES = 10 + 2;
const int SEED = 325;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASS w;
	w.lpszClassName = PROGRAM_TITLE;
	w.hInstance = hInstance;
	w.lpfnWndProc = WndProc;
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hIcon = 0;
	w.lpszMenuName = 0;
	w.hbrBackground = LTGRAY_BRUSH;
	w.style = CS_HREDRAW | CS_VREDRAW;
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;

	if (!RegisterClass(&w))
		return 0;

	wchar_t wchTitle[256];
	MultiByteToWideChar(CP_ACP, 0, PROGRAM_TITLE, -1, wchTitle, 256);

	HWND hWnd = CreateWindow(PROGRAM_TITLE,
		wchTitle,
		WS_OVERLAPPEDWINDOW,
		50,
		50,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		(HWND)NULL,
		(HMENU)NULL,
		(HINSTANCE)hInstance,
		(HINSTANCE)NULL);

	ShowWindow(hWnd, nCmdShow);


	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);

	ScriptParameters* args = malloc(sizeof(ScriptParameters));
	args->hWnd = &hWnd;
	args->windowHeight = WINDOW_HEIGHT;
	args->windowWidth = WINDOW_WIDTH;
	HANDLE scriptThread = CreateThread(NULL, 0, executeReportScript, args, 0, NULL);

	MSG msg;
	BOOL bRet;
	while ((bRet = GetMessage(&msg, hWnd, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			return(msg.wParam);
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;


	switch (msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		drawGraphs(&hdc);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return 0;
}

Graph* graph = NULL;
Graph* shadowGraph = NULL;
Graph* spanningTree = NULL;

DWORD WINAPI executeReportScript(LPVOID lpParam)
{
	ScriptParameters* args = lpParam;
	initializeLogConsole();
	double firstColumnX = args->windowWidth / 4;
	double secondColumnX = args->windowWidth / 4 * 3;
	double firstRowY = args->windowHeight / 2;

	int** matrix = getM(VERTICES, SEED, getK(SEED));
	matrix = symmetrify(matrix, VERTICES);
	graph = makeGraph(VERTICES, 30, 60, matrix,
		point(firstColumnX, firstRowY));
	shadowGraph = makeGraph(VERTICES, 30, 60, matrix,
		point(secondColumnX, firstRowY));
	printf("Graph adjacency matrix:\n\n");
	printMatrix(matrix, VERTICES);
	printf("\n\nGenrating a minimal spanning tree of the graph. Press any key to proceed.\n\n");
	pause(args->hWnd);
	int** treeMatrix = execute_prim_algo(args->hWnd, &spanningTree, matrix, 
		VERTICES, 30, 60, point(secondColumnX, firstRowY));
	printMatrix(treeMatrix, VERTICES);
}

void drawGraphs(HDC* hdc)
{
	HPEN blackPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	HPEN grayPen = CreatePen(PS_SOLID, 2, RGB(192, 192, 192));
	HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 50));
	if (graph != NULL)
		drawGraph(*hdc, blackPen, NULL, graph, 0);
	if (shadowGraph != NULL)
		drawGraph(*hdc, grayPen, NULL, shadowGraph, 0);
	if (spanningTree != NULL)
		drawGraph(*hdc, blackPen, NULL, spanningTree, 0);
}
