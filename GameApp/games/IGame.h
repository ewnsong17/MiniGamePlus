#pragma once
class IGamePlayer
{
public:
	BOOL player_alive;
	LPCWSTR player_name;
};

class IGame
{
public:

	std::vector<IGamePlayer> player_vector;
	INT player_turn;
	UINT player_size = 4;

	IGame();

	INT timer = 30;

	virtual VOID TurnCPU(HWND hWnd);

	virtual VOID SetNextTurn(HWND hWnd);

	virtual VOID ForceSetNextTurn(HWND hWnd);

	virtual VOID GetMouseClick(HWND hWnd, INT xPos, INT yPos);
};