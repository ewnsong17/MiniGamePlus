#pragma once

class InputClass;


class SystemClass
{
public:
	BOOL Initialize();
	VOID Shutdown();
	VOID Run();
	VOID InitialWindowButtons();
	VOID PaintScreen();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	BOOL Frame();
	VOID InitializeWindows();
	VOID ShutdownWindows();

private:
	LPCWSTR m_applicationName = L"�̴ϰ��Ӹ�����";
	HINSTANCE m_hinstance;
	WNDCLASSEX wnd_class;
	HWND m_hwnd;
	INT screenWidth = 1024;
	INT screenHeight = 768;

	InputClass* m_Input = nullptr;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = 0;