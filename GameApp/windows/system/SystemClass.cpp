#include "stdafx.h"
#include "input/InputClass.h"
#include "graphic/GraphicClass.h"
#include "system/SystemClass.h"

BOOL SystemClass::Initialize()
{
	//DirectX 그래픽 클래스 생성
	m_Graphic = new GraphicClass;

	if (!m_Graphic)
	{
		return FALSE;
	}

	HRESULT hr =m_Graphic->CreateDeviceIndependentResources(L"돋움", 26.f);

	if (SUCCEEDED(hr))
	{
		//윈도우 창 생성
		InitializeWindows();

		//키보드 입력 클래스 생성
		m_Input = new InputClass;

		if (!m_Input)
		{
			return FALSE;
		}

		//클래스 초기화
		m_Input->Initialize();

		return TRUE;
	}

	return FALSE;
}

VOID SystemClass::InitializeWindows()
{
	ApplicationHandle = this;

	m_hinstance = GetModuleHandle(NULL);

	wnd_class.cbSize = sizeof(WNDCLASSEX);
	wnd_class.cbClsExtra = 0;
	wnd_class.cbWndExtra = 0;
	wnd_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wnd_class.hInstance = m_hinstance;
	wnd_class.lpfnWndProc = WndProc;
	wnd_class.lpszClassName = m_applicationName;
	wnd_class.lpszMenuName = nullptr;
	wnd_class.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wnd_class);

	m_hwnd = CreateWindow(
		m_applicationName,
		m_applicationName,
		WS_SYSMENU | WS_OVERLAPPED,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		nullptr,
		nullptr,
		m_hinstance,
		nullptr
	);

	if (m_hwnd)
	{
		ShowWindow(m_hwnd, SW_SHOW);
		UpdateWindow(m_hwnd);
	}
}

VOID SystemClass::DeleteButtons(HWND hWnd)
{
	for (auto iter = m_EditCtrl.begin(); iter != m_EditCtrl.end(); iter++)
	{
		DestroyWindow(*iter);
	}
}

VOID SystemClass::CreateButtons(HWND hWnd)
{
	switch (m_stageCnt)
	{
		case START_GAME:
		{
			m_EditCtrl.push_back
			(
				CreateWindow(
					L"button",
					L"게임시작",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					270,
					675,
					100,
					30,
					hWnd,
					(HMENU)IDC_BTN_START,
					m_hinstance,
					nullptr
				)
			);

			m_EditCtrl.push_back
			(
				CreateWindow(
					L"button",
					L"종료하기",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					594,
					675,
					100,
					30,
					hWnd,
					(HMENU)IDC_BTN_END,
					m_hinstance,
					nullptr
				)
			);

			break;
		}
		case SELECT_GAME:
		{
			m_EditCtrl.push_back
			(
				CreateWindow(
					L"button",
					L"원카드",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					270,
					675,
					100,
					30,
					hWnd,
					(HMENU)IDC_GAME_CARD,
					m_hinstance,
					nullptr
				)
			);

			m_EditCtrl.push_back
			(
				CreateWindow(
					L"button",
					L"윷놀이",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					432,
					675,
					100,
					30,
					hWnd,
					(HMENU)IDC_GAME_YUT,
					m_hinstance,
					nullptr
				)
			);

			m_EditCtrl.push_back
			(
				CreateWindow(
					L"button",
					L"오목",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					594,
					675,
					100,
					30,
					hWnd,
					(HMENU)IDC_GAME_OMOK,
					m_hinstance,
					nullptr
				)
			);

			break;
		}
	}
}

VOID SystemClass::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (TRUE)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!Frame())
			{
				break;
			}
		}
	}
}

VOID SystemClass::Shutdown()
{
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	ShutdownWindows();
}

VOID SystemClass::ShutdownWindows()
{
	DestroyWindow(m_hwnd);
	m_hwnd = nullptr;

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = nullptr;

	ApplicationHandle = nullptr;
}

BOOL SystemClass::Frame()
{
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return FALSE;
	}
	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
		case WM_CREATE:
			ApplicationHandle->CreateButtons(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return ApplicationHandle->MessageHandler(hWnd, uMessage, wParam, lParam);
	}
	return 0;
}