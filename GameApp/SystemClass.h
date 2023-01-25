#pragma once

class InputClass;


class SystemClass
{
public:
	BOOL Initialize();
	VOID Shutdown();
	VOID Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	VOID InitialWindowButtons(HWND hWnd);

private:
	BOOL Frame();
	VOID InitializeWindows();
	VOID ShutdownWindows();

private:
	LPCWSTR m_applicationName = L"미니게임마스터";
	HINSTANCE m_hinstance;
	WNDCLASSEX wnd_class;
	HWND m_hwnd;

	InputClass* m_Input = nullptr;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = 0;