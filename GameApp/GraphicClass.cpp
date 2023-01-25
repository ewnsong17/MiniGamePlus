#include "stdafx.h"
#include "GraphicClass.h"

VOID GraphicClass::PaintScreen(HWND hWnd)
{
	PAINTSTRUCT ps;
	SetMemory(hWnd, &ps);

	SetBkMode(hDC, TRANSPARENT);
	PaintBackGround();

	Release(hWnd, &ps);
}

VOID GraphicClass::SetMemory(HWND hWnd, PAINTSTRUCT* ps)
{
	hDC = BeginPaint(hWnd, ps);
	hMemDC = CreateCompatibleDC(hDC);
	hBackMemDC = CreateCompatibleDC(hDC);
}

VOID GraphicClass::Release(HWND hWnd, PAINTSTRUCT* ps)
{
	DeleteDC(hMemDC);
	DeleteDC(hBackMemDC);

	EndPaint(hWnd, ps);
}

VOID GraphicClass::PaintBackGround()
{
	BITMAP bit;

	//double-buffer

	//Screen-Size ��ŭ�� ��Ʈ�� ����
	HBITMAP backBitMap = CreateCompatibleBitmap(hDC, SCREEN_WIDTH, SCREEN_HEIGHT);

	//������ ��Ʈ���� hBackMemDC�� ����
	HBITMAP hOldBitMap = (HBITMAP)SelectObject(hBackMemDC, backBitMap);

	//�̹��� ��Ʈ�� ������� �ε�
	HBITMAP hBitMap = (HBITMAP)LoadImage(nullptr, L"background.jpg", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	//BITMAP-bit�� ����
	GetObject(hBitMap, sizeof(bit), &bit);

	//������ ��Ʈ���� hMemDC�� ����
	HBITMAP hOld = (HBITMAP)SelectObject(hMemDC, hBitMap);

	//hMemDC�� ������ ���� �̹��� ���� hBackMemDC�� ����
	BitBlt(hBackMemDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hMemDC, 0, 0, SRCCOPY);

	//hBackMemDC�� ������ �̹����� ���� ���ȭ�� hDC�� ����
	BitBlt(hDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hBackMemDC, 0, 0, SRCCOPY);

	//hOld�� ����� hBitMap ���� ���� ������ �ٽ� ���� (�޸� ó���� ����)
	SelectObject(hBackMemDC, hOld);

	//�޸� ����
	DeleteObject(hBitMap);
}