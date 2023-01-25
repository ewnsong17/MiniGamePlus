#pragma once

class GraphicClass
{
public:
	VOID PaintScreen(HWND hWnd);
	VOID PaintBackGround();

	VOID SetMemory(HWND hWnd, PAINTSTRUCT* ps);
	VOID Release(HWND hWnd, PAINTSTRUCT* ps);

private:
	HDC hDC, hMemDC, hBackMemDC;
};

static GraphicClass* GraphicHandle = new GraphicClass();