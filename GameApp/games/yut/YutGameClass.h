#pragma once
#include "IGame.h"

class YutPlayer
{
public:
	INT owner;
	INT pos = -1;
	BOOL bEnter = FALSE;
	BOOL bClear = FALSE;
};

class YutGameClass : public IGame
{
public:
	std::vector<YutPlayer*> yut_player_vec;

	std::map<INT, std::wstring> yut_info
	{
		{2, L"»ªµµ"},
		{15, L"µµ"},
		{25, L"°³"},
		{25, L"°É"},
		{7, L"À·"},
		{4, L"¸ð"},
	};

	std::wstring yut_type = L"";

	YutGameClass(UINT player_size);

	VOID ThrowYut(HWND hWnd);

	virtual VOID GetMouseClick(HWND hWnd, INT xPos, INT yPos);

	virtual VOID TurnCPU(HWND hWnd);

	virtual VOID SetNextTurn(HWND hWnd);
};