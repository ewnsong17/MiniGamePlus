#include "stdafx.h"
#include "YutGameClass.h"
#include <system/SystemClass.h>

YutGameClass::YutGameClass(UINT player_size)
{
	//부모 생성자에서 할 것들 빼고 하기

	//1. 최대치보다 적을 경우 갯수 줄이기. (원카드는 해당 사항 없지만 일단 공통 처리)
	this->player_size = player_size;

	this->player_vector.resize(player_size);

	//2. 윷 말 지정
	for (int i = 0; i < player_size; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			yut_player_vec.push_back(new YutPlayer{ i , j});
		}
	}

	//3. 시작 플레이어 재설정
	unsigned int rand;
	rand_s(&rand);

	player_turn = rand % player_size;

	//4. 게임 타이머 세팅

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

			//TODO:: 플레이어 말 이동

			//TODO:: 윷, 모, 빽도 처리

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