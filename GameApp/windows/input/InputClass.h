#pragma once
#include <IGame.h>

class InputClass
{
public:
	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

	void MouseDown(unsigned int, unsigned int);
	void MouseUp(HWND hWnd, unsigned int, unsigned int, IGame*);

private:
	bool m_keys[256];
};