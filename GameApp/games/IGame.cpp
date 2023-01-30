#include "stdafx.h"
#include "IGame.h"

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

	//�׻� �� �տ� �ִ� �÷��̾� ���� ������.
	player_turn = 0;
}