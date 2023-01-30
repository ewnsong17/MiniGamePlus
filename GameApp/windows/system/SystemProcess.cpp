#include "stdafx.h"
#include "input/InputClass.h"
#include "graphic/GraphicClass.h"
#include "system/SystemClass.h"
#include "card/CardGameClass.h"

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
		case WM_PAINT:
			m_Graphic->OnRender(hWnd, m_stageCnt);
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
	DeleteButtons(hWnd);
	InvalidateRect(hWnd, nullptr, TRUE);

	switch (LOWORD(wParam))
	{
		case IDC_BTN_START:
			m_stageCnt = SELECT_GAME;
			CreateButtons(hWnd);
			break;
		case IDC_BTN_END:
			PostMessage(hWnd, WM_DESTROY, wParam, lParam);
			break;
		case IDC_GAME_CARD:
			m_stageCnt = CARD_GAME;
			m_Game = new CardGameClass();
			break;
		case IDC_GAME_YUT:
			m_stageCnt = YUT_GAME;
			break;
		case IDC_GAME_OMOK:
			m_stageCnt = OMOK_GAME;
			break;
	}
}