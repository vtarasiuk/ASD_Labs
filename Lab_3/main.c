#include<windows.h>
#include<stdio.h>
#include<math.h>

#include "graph.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void makeMatrix();

char* PROGRAM_TITLE = "LAB 3";
const int WIDTH = 1500;
const int HEIGHT = 800;
const int V = 12;
int** M = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	if (AllocConsole()) {
		FILE* pCout;
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		SetConsoleTitle(L"Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}

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
		WIDTH,
		HEIGHT,
		(HWND)NULL,
		(HMENU)NULL,
		(HINSTANCE)hInstance,
		(HINSTANCE)NULL);

	makeMatrix();

	ShowWindow(hWnd, nCmdShow);
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


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	static LOGFONT lf;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	lf.lfWeight = 10;
	HFONT hFont;
	hFont = CreateFontIndirect(&lf);
	HDC hdc;

	PAINTSTRUCT ps;


	switch (msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SelectObject(hdc, hFont);
		HPEN pen = CreatePen(PS_INSIDEFRAME, 3, RGB(0, 0, 0));

		RECT rect;
		GetWindowRect(hWnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		int** matrix = randomizeM(V, 325);

		Graph* oriented = makeGraph(V, 25, 80, matrix,
			point(width / 4 + 20, height / 2 + 50));
		drawGraph(hdc, pen, oriented, 1);

		int** symmetricMatrix = makeSymmetricM(matrix, V);
		Graph* nonOriented = makeGraph(V, 25, 80, symmetricMatrix,
			point(width / 4 * 3, height / 2 + 50));
		drawGraph(hdc, pen, nonOriented, 0);

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

void makeMatrix()
{
	int** M = randomizeM(V, 325);
	printf("M:\n");
	for (int i = 0; i < V; i++)
	{
		printf("%i ", i + 1);
	}
	printf("\n\n ");
	for (int i = 0; i < V; i++)
	{
		printf("%i ", i + 1);
		for (int j = 0; j < V; j++)
		{
			printf("%i ", M[i][j]);
		}
		printf("\n");
	}

	int** symM = makeSymmetricM(M, V);
	printf("\n\n\nSymmetric M:\n ");
	for (int i = 0; i < V; i++)
	{
		printf("%i ", i + 1);
	}
	printf("\n\n");
	for (int i = 0; i < V; i++)
	{
		printf("%i ", i + 1);
		for (int j = 0; j < V; j++)
		{
			printf("%i ", symM[i][j]);
		}
		printf("\n");
	}
}
