#include "stdafx.h"
#include "SystemClass.h"
#include "GraphicClass.h"
#include "InputClass.h"

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
		case WM_PAINT:
			m_Graphic->PaintScreen(hWnd);
			break;
		case WM_KEYDOWN:
			m_Input->KeyDown((unsigned int)wParam);
			break;
		case WM_KEYUP:
			m_Input->KeyUp((unsigned int)wParam);
			break;
		case WM_COMMAND:
			CommandHandler(hWnd, uMessage, wParam, lParam);
			break;
		default:
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	return 0;
}

VOID SystemClass::CommandHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case IDC_BTN_END:
			PostMessage(hWnd, WM_DESTROY, wParam, lParam);
			break;
	}
}