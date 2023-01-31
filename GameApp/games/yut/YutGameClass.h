#pragma once
#include "IGame.h"

class YutGameClass : public IGame
{
public:
	YutGameClass(UINT player_size);

	virtual VOID GetMouseClick(HWND hWnd, INT xPos, INT yPos);

	virtual VOID TurnCPU(HWND hWnd);

	virtual VOID SetNextTurn(HWND hWnd);
};