#include "stdafx.h"
#include "YutGameClass.h"
#include <system/SystemClass.h>

YutGameClass::YutGameClass(UINT player_size)
{
	//�θ� �����ڿ��� �� �͵� ���� �ϱ�

	//1. �ִ�ġ���� ���� ��� ���� ���̱�. (��ī��� �ش� ���� ������ �ϴ� ���� ó��)
	this->player_size = player_size;

	this->player_vector.resize(player_size);

	//2. �� �� ����
	for (int i = 0; i < player_size; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			yut_player_vec.push_back(new YutPlayer{ i , j});
		}
	}

	//3. ���� �÷��̾� �缳��
	unsigned int rand;
	rand_s(&rand);

	player_turn = rand % player_size;

	//4. ���� Ÿ�̸� ����

	DWORD threadId = 0;
	DWORD threadParam = 0;
	CreateThread(nullptr, 0, SystemClass::GameMainThread, &threadParam, 0, &threadId);
}

VOID YutGameClass::ThrowYut(HWND hWnd)
{
	UINT rand_;

	rand_s(&rand_);

	INT rand = (int) (rand_ % 60);

	for (auto iter = yut_info.begin(); iter != yut_info.end(); iter++)
	{
		rand -= iter->second;

		if (rand <= 0)
		{
			yut_type = iter->first;

			//TODO:: �÷��̾� �� �̵�

			//TODO:: ��, ��, ���� ó��

			SetNextTurn(hWnd);
			break;
		}
	}
}

VOID YutGameClass::GetMouseClick(HWND hWnd, INT xPos, INT yPos)
{
	if (!player_turn)
	{
		D2D1_POINT_2F ltSize = D2D1::Point2F(902.5f, 475.f);

		float radius = 50.f;

		float xVal = std::pow(static_cast<float>(xPos) - ltSize.x, 2);
		float yVal = std::pow(static_cast<float>(yPos) - ltSize.y, 2);

		if ((xVal / std::pow(radius, 2)) + (yVal / std::pow(radius, 2)) <= 1)
		{
			ThrowYut(hWnd);
		}
	}
}

VOID YutGameClass::TurnCPU(HWND hWnd)
{
	ThrowYut(hWnd);
}

VOID YutGameClass::SetNextTurn(HWND hWnd)
{
	player_turn = !player_turn;

	timer = 30;

	InvalidateRect(hWnd, nullptr, TRUE);
}