#include "stdafx.h"
#include "OmokGameClass.h"
#include <system/SystemClass.h>

OmokGameClass::OmokGameClass(UINT player_size)
{
	//부모 생성자에서 할 것들 빼고 하기

	//1. 최대치보다 적을 경우 갯수 줄이기. (원카드는 해당 사항 없지만 일단 공통 처리)
	this->player_size = player_size;

	this->player_vector.resize(player_size);

	//2. 바둑돌 위치 지정
	float len = 750.f;
	for (float x = 15.f; x <= len; x += 35)
	{
		for (float y = 15.f; y <= len; y += 35)
		{
			pos_list.push_back(new OmokPos{x, y});
		}
	}

	//3. 시작 플레이어 재설정

	player_turn = rand() % player_size;

	//4. 게임 타이머 세팅

	DWORD threadId = 0;
	DWORD threadParam = 0;
	CreateThread(nullptr, 0, SystemClass::GameMainThread, &threadParam, 0, &threadId);
}

VOID OmokGameClass::GetMouseClick(HWND hWnd, INT xPos, INT yPos)
{
	if (player_turn == 0 && timer > 0)
	{
		float val = 25;
		int index = -1;
		for (auto iter = pos_list.begin(); iter != pos_list.end(); iter++)
		{
			float _x = std::abs(xPos - (*iter)->x);
			float _y = std::abs(yPos - (*iter)->y);

			if (val > _x + _y)
			{
				val = _x + _y;

				index = iter - pos_list.begin();
			}
		}

		if (index >= 0)
		{
			pos_list[index]->bClick = CLICK_BLACK;

			InvalidateRect(hWnd, nullptr, TRUE);

			SetNextTurn(hWnd);
		}
	}
}

VOID OmokGameClass::SetRandPos()
{
	BOOL bCheck = FALSE;
	while (!bCheck)
	{
		int index = rand() % pos_list.size();
		if (pos_list[index]->bClick == CLICK_NONE)
		{
			bCheck = TRUE;
			pos_list[index]->bClick = player_turn;
		}
	}
}

VOID OmokGameClass::TurnCPU(HWND hWnd)
{
	//지능 추가
	//우선순위

	pos_list[100]->bClick = player_turn;
	pos_list[100 + 23]->bClick = player_turn;
	pos_list[100 + 46]->bClick = player_turn;

	int index = -1;

	for (int i = 0; i < pos_list.size(); i++)
	{
		//놓을 위치 찾았으면 그만 하기.
		if (index >= 0)
		{
			break;
		}


		int x_ = i + 88;
		if (pos_list.size() > x_)
		{
			//양 플레이어 동시 검색 가능하므로 동시 검색
			for (int turn = 0; turn < player_size; turn++)
			{
				//위치당 한쪽이상 안막힌 3개가 있는지 검색
				if (pos_list[i]->bClick == turn
					&& pos_list[(long long)i + 22]->bClick == turn
					&& pos_list[(long long)i + 44]->bClick == turn
					)
				{

					//4번째 위치도 안막혔는지 체크
					if (pos_list[(long long)i + 66]->bClick == turn)
					{
						//1. 4개에서 5개가 가능하면 제일 먼저 한다.
						//2. 상대의 4개를 무조건 막는다.
						if (i - 22 >= 0)
						{
							if (pos_list[(long long)i - 22]->bClick == CLICK_NONE)
							{
								index = i - 22;
							}
						}

						if (pos_list[(long long)i + 88]->bClick == CLICK_NONE)
						{
							index = i + 88;
						}
					}
					else
					{
						//3. 내 양쪽 안막힌 3개를 잇는다.
						//4. 내 한쪽 안막힌 3개를 잇는다.

						index = i + 66;
						break;
					}
				}

				if (turn == player_turn && index >= 0)
				{
					break;
				}
			}
		}

		int y_ = i + 4;
		if (pos_list.size() > y_)
		{
			for (int turn = 0; turn < player_size; turn++)
			{
				if (pos_list[i]->bClick == turn
					&& pos_list[(long long)i + 1]->bClick == turn
					&& pos_list[(long long)i + 2]->bClick == turn
					)
				{
					if (pos_list[(long long)i + 3]->bClick == turn)
					{

						if (i - 1 >= 0)
						{
							if (pos_list[(long long)i - 1]->bClick == CLICK_NONE)
							{
								index = i - 1;
							}
						}

						if (pos_list[(long long)i + 4]->bClick == CLICK_NONE)
						{
							index = i + 4;
						}
					}
					else
					{
						index = i + 3;
						break;
					}
				}

				if (turn == player_turn && index >= 0)
				{
					break;
				}
			}
		}

		int rb = i + 92;
		if (pos_list.size() > rb)
		{
			for (int turn = 0; turn < player_size; turn++)
			{
				if (pos_list[i]->bClick == turn
					&& pos_list[(long long)i + 23]->bClick == turn
					&& pos_list[(long long)i + 46]->bClick == turn
					)
				{
					if (pos_list[(long long)i + 69]->bClick == turn)
					{
						if (i - 23 >= 0)
						{
							if (pos_list[(long long)i - 23]->bClick == CLICK_NONE)
							{
								index = i - 23;
							}
						}

						if (pos_list[(long long)i + 92]->bClick == CLICK_NONE)
						{
							index = i + 92;
						}
					}
					else
					{
						index = i + 69;
						break;
					}
				}

				if (turn == player_turn && index >= 0)
				{
					break;
				}
			}
		}


		int lb = i + 21;
		int lb2 = i - 84;
		if (lb2 >= 0 && pos_list.size() > lb)
		{
			for (int turn = 0; turn < player_size; turn++)
			{
				if (pos_list[i]->bClick == turn
					&& pos_list[(long long)i - 21]->bClick == turn
					&& pos_list[(long long)i - 42]->bClick == turn
					)
				{

					if (pos_list[(long long)i - 63]->bClick == turn)
					{
						if (pos_list[(long long)i + 21]->bClick == CLICK_NONE)
						{
							index = i + 21;
						}

						if (pos_list[(long long)i - 84]->bClick == CLICK_NONE)
						{
							index = i - 84;
						}
					}
					else
					{
						index = i - 63;
						break;
					}
				}

				if (turn == player_turn && index >= 0)
				{
					break;
				}
			}
		}
	}

	//5. 상대 양쪽 안막힌 3개를 막는다.
	//6. 상대 한쪽 안막힌 3개를 막는다.
	//7. 내 양쪽 안막힌 2개를 잇는다.
	//8. 내 돌 옆에 놓는다.

	std::cout << "index : " << index << '\n';

	if (index >= 0)
	{
		pos_list[index]->bClick = player_turn;
	}
	else
	{
		//9. 아무데나 놓는다.
		SetRandPos();
	}

	SetNextTurn(hWnd);
}

VOID OmokGameClass::SetNextTurn(HWND hWnd)
{
	if (CheckFinish(player_turn))
	{
		timer = -10000;
		return;
	}

	player_turn = !player_turn;

	timer = 30;

	InvalidateRect(hWnd, nullptr, TRUE);
}

BOOL OmokGameClass::CheckFinish(int player)
{
	//가로, 세로, 오른쪽 아래, 왼쪽 아래 체크
	for (int i = 0; i < pos_list.size(); i++)
	{
		int x_ = i + 88;
		if (pos_list.size() > x_)
		{
			if (pos_list[i]->bClick == player
				&& pos_list[(long long)i + 22]->bClick == player
				&& pos_list[(long long)i + 44]->bClick == player
				&& pos_list[(long long)i + 66]->bClick == player
				&& pos_list[(long long)i + 88]->bClick == player)
			{
				return TRUE;
			}
		}

		int y_ = i + 4;
		if (pos_list.size() > y_)
		{
			if (pos_list[i]->bClick == player
				&& pos_list[(long long)i + 1]->bClick == player
				&& pos_list[(long long)i + 2]->bClick == player
				&& pos_list[(long long)i + 3]->bClick == player
				&& pos_list[(long long)i + 4]->bClick == player)
			{
				return TRUE;
			}
		}

		int rb = i + 92;
		if (pos_list.size() > rb)
		{
			if (pos_list[i]->bClick == player
				&& pos_list[(long long)i + 23]->bClick == player
				&& pos_list[(long long)i + 46]->bClick == player
				&& pos_list[(long long)i + 69]->bClick == player
				&& pos_list[(long long)i + 92]->bClick == player)
			{
				return TRUE;
			}
		}


		int lb = i - 84;
		if (pos_list.size() > lb)
		{
			if (pos_list[i]->bClick == player
				&& pos_list[(long long)i - 21]->bClick == player
				&& pos_list[(long long)i - 42]->bClick == player
				&& pos_list[(long long)i - 63]->bClick == player
				&& pos_list[(long long)i - 84]->bClick == player)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}