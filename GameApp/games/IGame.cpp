#include "stdafx.h"
#include "IGame.h"
#include <card/CardGameClass.h>
#include <omok/OmokGameClass.h>

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

	//먼저 시작할 사람 랜덤으로 정하기
	unsigned int rand;
	rand_s(&rand);

	player_turn = rand % player_size;
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

	//오목 처리
	if (OmokGameClass* omokGame = dynamic_cast<OmokGameClass*>(this))
	{
		if (omokGame != nullptr)
		{
			omokGame->TurnCPU(hWnd);
		}
	}
}

VOID IGame::ForceSetNextTurn(HWND hWnd)
{
	//원카드 처리
	if (CardGameClass* cardGame = dynamic_cast<CardGameClass*>(this))
	{
		if (cardGame != nullptr)
		{
			cardGame->GetCardFromGraves(player_turn);
			cardGame->SetNextTurn(hWnd);
		}
	}

	//오목 처리
	if (OmokGameClass* omokGame = dynamic_cast<OmokGameClass*>(this))
	{
		if (omokGame != nullptr)
		{
			omokGame->SetRandPos();
			omokGame->SetNextTurn(hWnd);
		}
	}
}

VOID IGame::SetNextTurn(HWND hWnd)
{
	//원카드 처리
	if (CardGameClass* cardGame = dynamic_cast<CardGameClass*>(this))
	{
		if (cardGame != nullptr)
		{
			cardGame->SetNextTurn(hWnd);
		}
	}

	//오목 처리
	if (OmokGameClass* omokGame = dynamic_cast<OmokGameClass*>(this))
	{
		if (omokGame != nullptr)
		{
			omokGame->SetNextTurn(hWnd);
		}
	}
}

VOID IGame::GetMouseClick(HWND hWnd, INT xPos, INT yPos)
{
	//원카드 처리
	if (CardGameClass* cardGame = dynamic_cast<CardGameClass*>(this))
	{
		if (cardGame != nullptr)
		{
			cardGame->GetMouseClick(hWnd, xPos, yPos);
		}
	}

	//오목 처리
	if (OmokGameClass* omokGame = dynamic_cast<OmokGameClass*>(this))
	{
		if (omokGame != nullptr)
		{
			omokGame->GetMouseClick(hWnd, xPos, yPos);
		}
	}
}