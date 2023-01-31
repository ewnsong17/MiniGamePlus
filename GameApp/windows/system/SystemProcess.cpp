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
			m_Graphic->OnRender(hWnd, m_stageCnt, m_Game);
			break;
		case WM_ERASEBKGND:
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
		case WM_LBUTTONUP:
			m_Input->MouseUp(hWnd, (unsigned int)wParam, (unsigned int)lParam, m_Game);
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
	DeleteButtons();

	switch (LOWORD(wParam))
	{
		case IDC_BTN_START:
			m_stageCnt = SELECT_GAME;
			CreateButtons(hWnd);
			InvalidateRect(hWnd, nullptr, TRUE);
			break;
		case IDC_BTN_END:
			PostMessage(hWnd, WM_DESTROY, wParam, lParam);
			break;
		case IDC_GAME_CARD:
			m_stageCnt = CARD_GAME;
			m_Game = new CardGameClass(4);
			CreateButtons(hWnd);
			InvalidateRect(hWnd, nullptr, TRUE);
			break;
		case IDC_CARD_CLOVER:
		case IDC_CARD_DIAMOND:
		case IDC_CARD_HEART:
		case IDC_CARD_SPADE:
			ShowColorButtons(FALSE);
			if (CardGameClass* cardGame = dynamic_cast<CardGameClass*>(m_Game))
			{
				if (cardGame != nullptr)
				{
					cardGame->SetColor(hWnd, LOWORD(wParam));
				}
			}
			break;
		case IDC_GAME_YUT:
			m_stageCnt = YUT_GAME;
			InvalidateRect(hWnd, nullptr, TRUE);
			break;
		case IDC_GAME_OMOK:
			m_stageCnt = OMOK_GAME;
			InvalidateRect(hWnd, nullptr, TRUE);
			break;
	}
}