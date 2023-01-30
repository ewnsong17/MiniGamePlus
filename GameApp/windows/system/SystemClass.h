#pragma once
#include "IGame.h"

class GraphicClass;
class InputClass;


class SystemClass
{
public:
	BOOL Initialize();
	VOID Shutdown();
	VOID Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	VOID CommandHandler(HWND, UINT, WPARAM, LPARAM);
	VOID CreateButtons(HWND hWnd);
	VOID DeleteButtons(HWND hWnd);

private:
	BOOL Frame();
	VOID InitializeWindows();
	VOID ShutdownWindows();

private:
	LPCWSTR m_applicationName = L"�̴ϰ��Ӹ�����";
	UINT m_stageCnt = 0;
	HINSTANCE m_hinstance;
	WNDCLASSEX wnd_class;
	HWND m_hwnd;

	InputClass* m_Input = nullptr;
	GraphicClass* m_Graphic = nullptr;

	std::vector<HWND> m_EditCtrl;

	IGame* m_Game = nullptr;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = 0;