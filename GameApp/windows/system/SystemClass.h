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
	VOID DeleteButtons();
	VOID ShowButtons();

	static DWORD WINAPI GameMainThread(LPVOID lpParam);
	static VOID ShowColorButtons(BOOL bShow);

private:
	BOOL Frame();
	VOID InitializeWindows();
	VOID ShutdownWindows();

	VOID EndGame();

private:
	LPCWSTR m_applicationName = L"미니게임마스터";
	UINT m_stageCnt = 0;
	HWND m_hwnd;
	HINSTANCE m_hinstance;
	WNDCLASSEX wnd_class;

	InputClass* m_Input = nullptr;
	GraphicClass* m_Graphic = nullptr;

	std::vector<HWND> m_EditCtrl;

	IGame* m_Game = nullptr;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static std::vector<HWND> m_ColorCtrl;
static SystemClass* ApplicationHandle = 0;