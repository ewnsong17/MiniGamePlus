#pragma once
#include "IGame.h"

class YutPlayer
{
public:
	INT owner;
	INT index;
	INT pos = -1;
	BOOL bEnter = FALSE;
	BOOL bClear = FALSE;
};

class YutGameClass : public IGame
{
public:
	std::vector<YutPlayer*> yut_player_vec;

	std::map<std::wstring, INT> yut_info
	{
		{L"»ªµµ", 2},
		{L"µµ", 12},
		{L"°³", 18},
		{L"°É", 18},
		{L"À·", 7},
		{L"¸ð", 3},
	};

	std::wstring system_message = L"";

	std::wstring yut_type = L"";

	YutGameClass(UINT player_size);

	~YutGameClass();

	VOID ThrowYut(HWND hWnd);

	VOID SetPlayerYutMoveList(HWND hWnd);

	virtual VOID GetMouseClick(HWND hWnd, INT xPos, INT yPos);

	virtual VOID TurnCPU(HWND hWnd);

	virtual VOID SetNextTurn(HWND hWnd);
};