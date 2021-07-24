#include <windows.h>
#include <stdio.h>
#include <math.h>

#include "drawing.h"
#include "memory.h"
#include "algorithms.h"


typedef struct {
	HWND* hWnd;
	int windowWidth;
	int windowHeight;
} ScriptParameters;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI executeReportScript(LPVOID lpParam);
void drawGraphs1(HDC* hdc);
void drawGraphs2(HDC* hdc);

char* PROGRAM_TITLE = "LAB 5";
const int WINDOW_WIDTH = 1500;
const int WINDOW_HEIGHT = 800;
const int VERTICES = 12;
const int SEED = 325;


void initializeLogConsole()
{
	if (AllocConsole()) {
		FILE* pCout;
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		SetConsoleTitle(L"Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
	system("cls");
}

void printMatrix(int** matrix, int vertices)
{
	printf("#\t");
	for (int i = 0; i < vertices; i++)
	{
		printf("%i\t", i + 1);
	}
	printf("\n\n");
	for (int i = 0; i < vertices; i++)
	{
		printf("%i\t", i + 1);
		for (int j = 0; j < vertices; j++)
		{
			printf("%i\t", matrix[i][j]);
		}
		printf("\n");
	}
}

void printLine(int length)
{
	printf("\n\n");
	for (int i = 0; i < length; i++)
		printf("*");
	printf("\n\n");
}

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

int scriptSwitch = 1;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;


	switch (msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (scriptSwitch == 1)
			drawGraphs1(&hdc);
		else if (scriptSwitch == 2)
			drawGraphs2(&hdc);
		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		scriptSwitch = (scriptSwitch == 1) ? 2 : 1;
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return 0;
}

Graph* dfsGraph = NULL;
Graph* dfsTree = NULL;
Graph* bfsGraph = NULL;
Graph* bfsTree = NULL;

DWORD WINAPI executeReportScript(LPVOID lpParam)
{
	ScriptParameters* args = lpParam;
	initializeLogConsole();
	double firstColumnX = args->windowWidth / 4;
	double secondColumnX = args->windowWidth / 4 * 3;
	double firstRowY = args->windowHeight / 2;

	int** matrix = createMatrix(VERTICES, SEED, k(SEED));

	dfsGraph = generateGraph(VERTICES, 30, 60, matrix,
		point(firstColumnX, firstRowY));
	printf("First matrix:\n\n");
	printMatrix(dfsGraph->matrix, VERTICES);
	printf("\nDFS... [press key to next step]\n");
	int** dfsTreeMatrix = dfs(args->hWnd, dfsGraph->vertices, matrix, VERTICES);
	dfsTree = generateGraph(VERTICES, 30, 60, dfsTreeMatrix,
		point(secondColumnX, firstRowY));
	for (int i = 0; i < VERTICES; i++)
		dfsTree->vertices[i]->title = i + 1;

	printLine(90);
	bfsGraph = generateGraph(VERTICES, 30, 60, matrix,
		point(firstColumnX, firstRowY));
	printf("Second matrix:\n\n");
	printMatrix(matrix, VERTICES);
	printf("\nBFS... [press key to next step]\n");
	int** bfsTreeMatrix = bfs(args->hWnd, bfsGraph->vertices, matrix, VERTICES);
	bfsTree = generateGraph(VERTICES, 30, 60, bfsTreeMatrix,
		point(secondColumnX, firstRowY));
	for (int i = 0; i < VERTICES; i++)
		bfsTree->vertices[i]->title = i + 1;
}

void drawGraphs1(HDC* hdc)
{
	HPEN black = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	HPEN red = CreatePen(PS_SOLID, 4, RGB(255, 0, 50));
	if (dfsGraph != NULL)
		drawGraph(*hdc, black, red, dfsGraph, 1);
	if (dfsTree != NULL)
		drawGraph(*hdc, black, NULL, dfsTree, 1);
}



void drawGraphs2(HDC* hdc)
{
	HPEN black = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	HPEN red = CreatePen(PS_SOLID, 4, RGB(255, 0, 50));
	if (bfsGraph != NULL)
		drawGraph(*hdc, black, red, bfsGraph, 1);
	if (bfsTree != NULL)
		drawGraph(*hdc, black, NULL, bfsTree, 1);
}
