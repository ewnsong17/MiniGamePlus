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
		{2, L"����"},
		{15, L"��"},
		{25, L"��"},
		{25, L"��"},
		{7, L"��"},
		{4, L"��"},
	};

	std::wstring yut_type = L"";

	YutGameClass(UINT player_size);

	VOID ThrowYut(HWND hWnd);

	virtual VOID GetMouseClick(HWND hWnd, INT xPos, INT yPos);

	virtual VOID TurnCPU(HWND hWnd);

	virtual VOID SetNextTurn(HWND hWnd);
};