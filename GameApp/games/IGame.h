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
	UINT player_turn;
	UINT player_size = 4;

	IGame();

	UINT timer = 30;

	virtual VOID SetPlayerDead();

	virtual VOID TurnCPU(HWND hWnd);
};