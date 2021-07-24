#include <windows.h>
#include <stdio.h>
#include <math.h>

#include "drawing.h"
#include "memory.h"
#include "algo.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void firstReport(int windowWidth, int windowHeight);
void secondReport(int windowWidth, int windowHeight);
void drawGraphs1(HDC* hdc);
void drawGraphs2(HDC* hdc);

char* PROGRAM_TITLE = "LAB 4";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int VERTICES = 12;
const int SEED = 325;

void printM(int** M, int size)
{
	printf("#\t");
	for (int i = 0; i < size; i++)
	{
		printf("%i\t", i + 1);
	}
	printf("\n\n");
	for (int i = 0; i < size; i++)
	{
		printf("%i\t", i + 1);
		for (int j = 0; j < size; j++)
		{
			printf("%i\t", M[i][j]);
		}
		printf("\n");
	}
}

void delimeter(int length)
{
	printf("\n\n");
	for (int i = 0; i < length; i++)
		printf("+");
	printf("\n\n");
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
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

	firstReport(1500, 800);
	secondReport(1500, 800);

	UpdateWindow(hWnd);

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

Graph* oriented;
Graph* nonOriented;
Graph* modified;
Graph* condensed;

void firstReport(int windowWidth, int windowHeight)
{
	if (AllocConsole()) {
		FILE* pCout;
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		SetConsoleTitle(L"Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}

	double firstColumnX = windowWidth / 4 + 20;
	double secondColumnX = windowWidth / 4 * 3;
	double firstRowY = windowHeight / 2 + 50;

	//--------------ORIENTED----------------
	int** orientedMatrix = createMatrix(VERTICES, SEED, k1(SEED));
	oriented = generateGraph(VERTICES, 30, 60, orientedMatrix,
		point(firstColumnX, firstRowY));
	printf("M:\n\n\t");
	printM(orientedMatrix, VERTICES);

	int** orientedDegrees = or_degrees(orientedMatrix, VERTICES);
	delimeter(20);
	printf("Degrees:\n\n");
	printf("#\tIn\tOut\n");
	for (int i = 0; i < VERTICES; i++)
	{
		printf("%i\t%i\t\t%i\n", i + 1, orientedDegrees[i][1], orientedDegrees[i][0]);
	}

	delimeter(20);
	int orientedRegularityDegree = or_reg_degrees(orientedDegrees, VERTICES);
	if (orientedRegularityDegree == -1)
		printf("Oriented graph is not regular.");
	else
		printf("Oriented graph is %i-regular", orientedRegularityDegree);

	delimeter(20);
	printf("Sources:  ");
	for (int i = 0; i < VERTICES; i++)
	{
		if (orientedDegrees[i][1] == 0)
			printf("%i  ", i + 1);
	}
	printf("\nSinks:  ");
	for (int i = 0; i < VERTICES; i++)
	{
		if (orientedDegrees[i][0] == 0)
			printf("%i  ", i + 1);
	}
	printf("\nIsolates:  ");
	for (int i = 0; i < VERTICES; i++)
	{
		if (orientedDegrees[i][0] == 0 && orientedDegrees[i][1] == 0)
			printf("%i  ", i + 1);
	}

	//------------NON-ORIENTED--------------
	int** nonOrientedMatrix = simmetrify(orientedMatrix, VERTICES);
	nonOriented = generateGraph(VERTICES, 30, 60, nonOrientedMatrix,
		point(secondColumnX, firstRowY));
	delimeter(90);
	printf("Non-oriented M:\n\n\t");
	printM(nonOrientedMatrix, VERTICES);

	int* nonOrientedDegrees = nor_degrees(nonOrientedMatrix, VERTICES);
	delimeter(20);
	printf("Non-oriented degrees:\n\n");
	printf("#\tDegree\n");
	for (int i = 0; i < VERTICES; i++)
	{
		printf("%i\t%i\n", i + 1, nonOrientedDegrees[i]);
	}

	delimeter(20);
	int nonOrientedRegularityDegree = nor_reg_degree(nonOrientedDegrees, VERTICES);
	if (nonOrientedRegularityDegree == -1)
		printf("Non-oriented graph is not regular.");
	else
		printf("Non-oriented graph is %i-regular", nonOrientedRegularityDegree);

	delimeter(20);
	printf("Dangling vertices:  ");
	for (int i = 0; i < VERTICES; i++)
	{
		if (nonOrientedDegrees[i] == 1)
			printf("%i  ", i + 1);
	}
	printf("\nIsolated vertices:  ");
	for (int i = 0; i < VERTICES; i++)
	{
		if (orientedDegrees[i] == 0)
			printf("%i  ", i + 1);
	}

}

void drawGraphs1(HDC* hdc)
{
	HPEN bluePen = CreatePen(PS_SOLID, 3, RGB(50, 0, 0));
	drawGraph(*hdc, bluePen, oriented, 1);
	drawGraph(*hdc, bluePen, nonOriented, 0);
}

void secondReport(int windowWidth, int windowHeight)
{
	HPEN bluePen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
	double firstColumnX = windowWidth / 4 + 20;
	double secondColumnX = windowWidth / 4 * 3;
	double firstRowY = windowHeight / 2 + 50;

	//--------------MODIFIED----------------
	delimeter(90);
	int** modifiedMatrix = createMatrix(VERTICES, SEED, k2(SEED));
	modified = generateGraph(VERTICES, 30, 60, modifiedMatrix,
		point(firstColumnX, firstRowY));
	printf("Modified M:\n\n\t");
	printM(modifiedMatrix, VERTICES);

	int** modifiedDegrees = or_degrees(modifiedMatrix, VERTICES);
	delimeter(20);
	printf("Modified graph degrees:\n\n");
	printf("#\tIn\tOut\n");
	for (int i = 0; i < VERTICES; i++)
	{
		printf("%i\t%i\t\t%i\n", i + 1, modifiedDegrees[i][1], modifiedDegrees[i][0]);
	}

	int pathesOfLength2Amount;
	int** pathesOfLength2 = pathes_2(modifiedMatrix, VERTICES, &pathesOfLength2Amount);
	delimeter(20);
	printf("Pathes of length 2 in modified graph:\n");
	for (int i = 0; i < pathesOfLength2Amount; i++)
	{
		printf("%i %i %i\n",
			pathesOfLength2[i][0] + 1,
			pathesOfLength2[i][1] + 1,
			pathesOfLength2[i][2] + 1);
	}

	int pathesOfLength3Amount;
	int** pathesOfLength3 = pathes_3(modifiedMatrix, VERTICES, &pathesOfLength3Amount);
	delimeter(20);
	printf("Pathes of length 3 in modified graph:\n");
	for (int i = 0; i < pathesOfLength3Amount; i++)
	{
		printf("%i %i %i %i\n",
			pathesOfLength3[i][0] + 1,
			pathesOfLength3[i][1] + 1,
			pathesOfLength3[i][2] + 1,
			pathesOfLength3[i][3] + 1);
	}

	int** modifiedAccessibility = access_M(modifiedMatrix, VERTICES);
	delimeter(20);
	printf("Modified graph accessibility matrix:\n\n");
	printM(modifiedAccessibility, VERTICES);

	int** modifiedStrongConnectionMatrix = strong_connected_M(modifiedAccessibility, VERTICES);
	delimeter(20);
	printf("Modified graph strong connection matrix:\n\n");
	printM(modifiedStrongConnectionMatrix, VERTICES);

	int componentsAmount;
	int** modifiedGraphStrongConnectionComponents = strong_components(
		modifiedStrongConnectionMatrix,
		VERTICES,
		&componentsAmount);
	delimeter(20);
	printf("Modified graph strong connection components list:\n");
	for (int i = 0; modifiedGraphStrongConnectionComponents[i][0] != -1; i++)
	{
		printf("\nComponent #%i:   ", i + 1);
		for (int j = 0; modifiedGraphStrongConnectionComponents[i][j] != -1; j++)
		{
			printf("%i  ", modifiedGraphStrongConnectionComponents[i][j] + 1);
		}
	}

	int** modifiedGraphCondensedMatrix = condensed_M(
		modifiedGraphStrongConnectionComponents,
		componentsAmount,
		modifiedAccessibility,
		VERTICES);
	delimeter(20);
	printf("Modified graph condensed matrix:\n");
	printM(modifiedGraphCondensedMatrix, componentsAmount);


	condensed = generateGraph(componentsAmount, 30, 60, modifiedGraphCondensedMatrix,
		point(secondColumnX, firstRowY));
}



void drawGraphs2(HDC* hdc)
{
	HPEN pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	drawGraph(*hdc, pen, modified, 1);
	drawGraph(*hdc, pen, condensed, 1);
}
