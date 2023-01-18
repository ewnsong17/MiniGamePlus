#include "stdafx.h"
#include "InputClass.h"
#include "SystemClass.h"

BOOL SystemClass::Initialize()
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
		screenWidth,
		screenHeight,
		nullptr,
		nullptr,
		m_hinstance,
		nullptr
	);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
}

VOID SystemClass::InitialWindowButtons()
{

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
			ApplicationHandle->InitialWindowButtons();
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return ApplicationHandle->MessageHandler(hWnd, uMessage, wParam, lParam);
	}
	return 0;
}