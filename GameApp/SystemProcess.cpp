#include "stdafx.h"
#include "SystemClass.h"
#include "GraphicClass.h"
#include "InputClass.h"

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
		case WM_PAINT:
			m_Graphic->OnRenderImage(hWnd, D2D1::RectF(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT), TRUE);
			m_Graphic->OnRenderText(hWnd, L"미니게임마스터!", D2D1::SizeF(SCREEN_WIDTH, 200.f));
			ValidateRect(hWnd, nullptr);
			break;
		case WM_SIZE:
			m_Graphic->OnResize(LOWORD(lParam), HIWORD(lParam));
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
		case IDC_BTN_START:
//			m_Graphic->PaintText(hWnd, L"Ms Shell Dlg", 32.f, D2D1::ColorF(1.0f, 1.0f, 1.0f), L"미니게임마스터!");
			break;
		case IDC_BTN_END:
			PostMessage(hWnd, WM_DESTROY, wParam, lParam);
			break;
	}
}