#include "stdafx.h"
#include "IGame.h"
#include <card/CardGameClass.h>

IGame::IGame()
{
	//모든 게임 공통으로 필요한 것 처리.

	//1. 플레이어 리스트 처리
	player_vector.resize(player_size);

	LPCWSTR player_names[] = { L"나", L"CPU1", L"CPU2", L"CPU3" };

	//각 플레이어 부활시키고, 이름 새기기.
	for (int i = 0; i < player_size; i++)
	{
		player_vector[i].player_alive = TRUE;
		player_vector[i].player_name = player_names[i];
	}

	//항상 맨 앞에 있는 플레이어 부터 시작함.
	player_turn = 0;
}

VOID IGame::SetPlayerDead()
{

}

VOID IGame::TurnCPU(HWND hWnd)
{
	//원카드 처리
	if (CardGameClass* cardGame = dynamic_cast<CardGameClass*>(this))
	{
		if (cardGame != nullptr)
		{
			cardGame->TurnCPU(hWnd);
		}
	}
}