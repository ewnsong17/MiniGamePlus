#pragma once
#include "IGame.h"

constexpr int CLICK_NONE = -1;
constexpr int CLICK_BLACK = 0;
constexpr int CLICK_WHITE = 1;

class OmokPos
{
public:
	FLOAT x, y;
	INT bClick = CLICK_NONE;
};

class OmokGameClass : public IGame
{
public:
	OmokGameClass(UINT player_size);

	~OmokGameClass();

	virtual VOID GetMouseClick(HWND hWnd, INT xPos, INT yPos);

	virtual VOID TurnCPU(HWND hWnd);

	virtual VOID SetNextTurn(HWND hWnd);

	VOID SetRandPos();

	BOOL CheckFinish(INT player);

	std::vector<OmokPos*> pos_list;
};