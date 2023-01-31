#include "stdafx.h"
#include "IGame.h"
#include <card/CardGameClass.h>
#include <omok/OmokGameClass.h>

IGame::IGame()
{
	//��� ���� �������� �ʿ��� �� ó��.

	//1. �÷��̾� ����Ʈ ó��
	player_vector.resize(player_size);

	LPCWSTR player_names[] = { L"��", L"CPU1", L"CPU2", L"CPU3" };

	//�� �÷��̾� ��Ȱ��Ű��, �̸� �����.
	for (int i = 0; i < player_size; i++)
	{
		player_vector[i].player_alive = TRUE;
		player_vector[i].player_name = player_names[i];
	}

	//���� ������ ��� �������� ���ϱ�
	unsigned int rand;
	rand_s(&rand);

	player_turn = rand % player_size;
}

VOID IGame::TurnCPU(HWND hWnd)
{
	//��ī�� ó��
	if (CardGameClass* cardGame = dynamic_cast<CardGameClass*>(this))
	{
		if (cardGame != nullptr)
		{
			cardGame->TurnCPU(hWnd);
		}
	}

	//���� ó��
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
	//��ī�� ó��
	if (CardGameClass* cardGame = dynamic_cast<CardGameClass*>(this))
	{
		if (cardGame != nullptr)
		{
			cardGame->GetCardFromGraves(player_turn);
			cardGame->SetNextTurn(hWnd);
		}
	}

	//���� ó��
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
	//��ī�� ó��
	if (CardGameClass* cardGame = dynamic_cast<CardGameClass*>(this))
	{
		if (cardGame != nullptr)
		{
			cardGame->SetNextTurn(hWnd);
		}
	}

	//���� ó��
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
	//��ī�� ó��
	if (CardGameClass* cardGame = dynamic_cast<CardGameClass*>(this))
	{
		if (cardGame != nullptr)
		{
			cardGame->GetMouseClick(hWnd, xPos, yPos);
		}
	}

	//���� ó��
	if (OmokGameClass* omokGame = dynamic_cast<OmokGameClass*>(this))
	{
		if (omokGame != nullptr)
		{
			omokGame->GetMouseClick(hWnd, xPos, yPos);
		}
	}
}