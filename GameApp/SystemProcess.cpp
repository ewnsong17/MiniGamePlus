#include "stdafx.h"
#include "SystemClass.h"
#include "InputClass.h"
#include "GraphicClass.h"

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
		case WM_PAINT:
			GraphicHandle->PaintScreen(hWnd);
			break;
		case WM_KEYDOWN:
			m_Input->KeyDown((unsigned int)wParam);
			break;
		case WM_KEYUP:
			m_Input->KeyUp((unsigned int)wParam);
			break;
		default:
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	return 0;
}