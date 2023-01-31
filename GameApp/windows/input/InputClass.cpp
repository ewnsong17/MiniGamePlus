#include "stdafx.h"
#include "inputclass.h"

void InputClass::Initialize()
{
	// Ű �迭�� �ʱ�ȭ�մϴ�
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}
}


void InputClass::KeyDown(unsigned int input)
{
	// Ű�� ���ȴٸ� �ش� Ű���� true�� �����մϴ�
	m_keys[input] = true;
}


void InputClass::KeyUp(unsigned int input)
{
	// Ű�� �����Ǿ��ٸ� �ش� Ű���� false�� �����մϴ�
	m_keys[input] = false;
}


bool InputClass::IsKeyDown(unsigned int key)
{
	// ���� Ű���� ���������� �ƴ��� ���¸� ��ȯ�մϴ�
	return m_keys[key];
}

void InputClass::MouseDown(unsigned int input, unsigned int pos)
{
}

void InputClass::MouseUp(HWND hWnd, unsigned int input, unsigned int pos, IGame* game)
{
	int xPos = LOWORD(pos);
	int yPos = HIWORD(pos);

	if (game != nullptr)
	{
		game->GetMouseClick(hWnd, xPos, yPos);
	}
}