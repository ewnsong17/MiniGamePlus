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

	//Screen-Size 만큼의 비트맵 생성
	HBITMAP backBitMap = CreateCompatibleBitmap(hDC, SCREEN_WIDTH, SCREEN_HEIGHT);

	//생성한 비트맵을 hBackMemDC에 저장
	HBITMAP hOldBitMap = (HBITMAP)SelectObject(hBackMemDC, backBitMap);

	//이미지 비트맵 방식으로 로딩
	HBITMAP hBitMap = (HBITMAP)LoadImage(nullptr, L"background.jpg", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	//BITMAP-bit에 저장
	GetObject(hBitMap, sizeof(bit), &bit);

	//생성한 비트맵을 hMemDC에 저장
	HBITMAP hOld = (HBITMAP)SelectObject(hMemDC, hBitMap);

	//hMemDC에 저장한 실제 이미지 값을 hBackMemDC에 복사
	BitBlt(hBackMemDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hMemDC, 0, 0, SRCCOPY);

	//hBackMemDC에 복사한 이미지를 실제 배경화면 hDC에 복사
	BitBlt(hDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hBackMemDC, 0, 0, SRCCOPY);

	//hOld에 저장된 hBitMap 값을 이전 값으로 다시 변경 (메모리 처리를 위해)
	SelectObject(hBackMemDC, hOld);

	//메모리 해제
	DeleteObject(hBitMap);
}