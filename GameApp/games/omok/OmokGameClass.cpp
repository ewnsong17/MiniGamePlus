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
		for (float y = 15.f; y < len; y += 35)
		{
			pos_list.push_back(new OmokPos{x, y});
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

		if (index >= 0 && pos_list[index]->bClick == CLICK_NONE)
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
		unsigned int rand;
		rand_s(&rand);

		int index = rand % pos_list.size();
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

	int index = -1;
	int priority = 9;
	std::set<int> index_list;

	for (int i = 0; i < pos_list.size(); i++)
	{
		//가로 검색
		int x_ = i + 84;
		if (pos_list.size() > x_ && pos_list[i]->x <= 610.f)
		{
			//양 플레이어 동시 검색 가능하므로 동시 검색
			for (int turn = 0; turn < player_size; turn++)
			{
				//각 턴의 바둑알 검색
				if (pos_list[i]->bClick == turn)
				{
					//다음 위치도 안막혔는지 체크
					if (pos_list[(long long)i + 21]->bClick == turn)
					{
						//3번째 위치도 안막혔는지 체크
						if (pos_list[(long long)i + 42]->bClick == turn)
						{
							//4번째 위치도 안막혔는지 체크
							if (pos_list[(long long)i + 63]->bClick == turn)
							{
								//1. 상대의 4개를 무조건 막는다.
								//2. 내 4개를 무조건 잇는다.
								if (i - 21 >= 0)
								{
									if (pos_list[(long long)i - 21]->bClick == CLICK_NONE)
									{
										if (priority > (turn == player_turn ? 2 : 1))
										{
											priority = (turn == player_turn ? 2 : 1);
											index = i - 21;
											break;
										}
									}
								}

								if (pos_list[(long long)i + 84]->bClick == CLICK_NONE)
								{
									if (priority > (turn == player_turn ? 2 : 1))
									{
										priority = (turn == player_turn ? 2 : 1);
										index = i + 84;
										break;
									}
								}
							}
							else
							{
								//나 : ■, 상대 : ㅁ

								if (i - 21 >= 0)
								{
									INT left_turn = pos_list[(long long)i - 21]->bClick;
									INT right_turn = pos_list[(long long)i + 63]->bClick;

									if (left_turn == CLICK_NONE && right_turn == CLICK_NONE)
									{
										//3. 상대 양쪽 안막힌 3개를 막는다.
										// ㅁ -> xㅁㅁㅁx

										//5. 내 양쪽 안막힌 3개를 잇는다.
										// ■ -> x■■■x

										if (priority > (turn != player_turn) ? 3 : 5)
										{
											priority = (turn != player_turn) ? 3 : 5;
											index = (turn != player_turn) ? (i - 21) : (i + 63);
											break;
										}
									}
									else if (left_turn == CLICK_NONE && right_turn != CLICK_NONE)
									{
										//4. 상대 한쪽 안막힌 3개를 막는다.
										// ㅁ -> xㅁㅁㅁ■

										//6. 내 한쪽 안막힌 3개를 잇는다.
										// ■ -> x■■■ㅁ

										if (priority > (turn != player_turn) ? 4 : 6)
										{
											priority = (turn != player_turn) ? 4 : 6;
											index = i - 21;
											break;
										}
									}
									else if (left_turn == player_turn && right_turn == CLICK_NONE)
									{
										//4. 상대 한쪽 안막힌 3개를 막는다.
										// ㅁ -> ■ㅁㅁㅁx

										//6. 내 한쪽 안막힌 3개를 잇는다.
										// ■ -> ㅁ■■■x

										if (priority > (turn != player_turn) ? 4 : 6)
										{
											priority = (turn != player_turn) ? 4 : 6;
											index = i + 63;
											break;
										}
									}
								}
							}
						}
						else if (turn == player_turn && pos_list[(long long)i + 42]->bClick == CLICK_NONE)
						{
							//7. 내 양쪽 안막힌 2개를 잇는다.
							if (priority > 7)
							{
								priority = 7;
								index = i + 42;
								break;
							}
						}
					}
					else if (turn == player_turn && pos_list[(long long)i + 21]->bClick == CLICK_NONE)
					{
						//8. 내 돌 옆에 잇는다.
						if (priority >= 8)
						{
							priority = 8;
							index_list.insert(i + 21);
							break;
						}
					}
				}
			}
		}

		//세로 검색
		int y_ = i + 4;
		if (pos_list.size() > y_ && pos_list[i]->y < 610.f)
		{
			for (int turn = 0; turn < player_size; turn++)
			{
				if (pos_list[i]->bClick == turn)
				{
					if (pos_list[(long long)i + 1]->bClick == turn)
					{
						if (pos_list[(long long)i + 2]->bClick == turn)
						{
							if (pos_list[(long long)i + 3]->bClick == turn)
							{

								if (i - 1 >= 0)
								{
									if (pos_list[(long long)i - 1]->bClick == CLICK_NONE)
									{
										if (priority > (turn == player_turn ? 2 : 1))
										{
											priority = (turn == player_turn ? 2 : 1);
											index = i - 1;
											break;
										}
									}
								}

								if (pos_list[(long long)i + 4]->bClick == CLICK_NONE)
								{
									if (priority > (turn == player_turn ? 2 : 1))
									{
										priority = (turn == player_turn ? 2 : 1);
										index = i + 4;
										break;
									}
								}
							}
							else
							{
								if (i - 1 >= 0)
								{
									INT left_turn = pos_list[(long long)i - 1]->bClick;
									INT right_turn = pos_list[(long long)i + 3]->bClick;

									if (left_turn == CLICK_NONE && right_turn == CLICK_NONE)
									{
										//3. 상대 양쪽 안막힌 3개를 막는다.
										// ㅁ -> xㅁㅁㅁx

										//5. 내 양쪽 안막힌 3개를 잇는다.
										// ■ -> x■■■x

										if (priority > (turn != player_turn) ? 3 : 5)
										{
											priority = (turn != player_turn) ? 3 : 5;
											index = (turn != player_turn) ? (i - 1) : (i + 3);
											break;
										}
									}
									else if (left_turn == CLICK_NONE && right_turn != CLICK_NONE)
									{
										//4. 상대 한쪽 안막힌 3개를 막는다.
										// ㅁ -> xㅁㅁㅁ■

										//6. 내 한쪽 안막힌 3개를 잇는다.
										// ■ -> x■■■ㅁ

										if (priority > (turn != player_turn) ? 4 : 6)
										{
											priority = (turn != player_turn) ? 4 : 6;
											index = i - 1;
											break;
										}
									}
									else if (left_turn == player_turn && right_turn == CLICK_NONE)
									{
										//4. 상대 한쪽 안막힌 3개를 막는다.
										// ㅁ -> ■ㅁㅁㅁx

										//6. 내 한쪽 안막힌 3개를 잇는다.
										// ■ -> ㅁ■■■x

										if (priority > (turn != player_turn) ? 4 : 6)
										{
											priority = (turn != player_turn) ? 4 : 6;
											index = i + 6;
											break;
										}
									}
								}
							}
						}
						else if (turn == player_turn && pos_list[(long long)i + 2]->bClick == CLICK_NONE)
						{
							if (priority > 7)
							{
								priority = 7;
								index = i + 2;
								break;
							}
						}
					}
					else if (pos_list[(long long)i + 1]->bClick == CLICK_NONE)
					{
						if (priority >= 8)
						{
							priority = 8;
							index_list.insert(i + 1);
							break;
						}
					}
				}
			}
		}

		//우측 하단
		int rb = i + 88;
		if (pos_list.size() > rb && pos_list[i]->x <= 610.f && pos_list[i]->y < 610.f)
		{
			for (int turn = 0; turn < player_size; turn++)
			{
				if (pos_list[i]->bClick == turn)
				{
					if (pos_list[(long long)i + 22]->bClick == turn)
					{
						if (pos_list[(long long)i + 44]->bClick == turn)
						{
							if (pos_list[(long long)i + 66]->bClick == turn)
							{
								if (i - 22 >= 0)
								{
									if (pos_list[(long long)i - 22]->bClick == CLICK_NONE)
									{
										if (priority > (turn == player_turn ? 2 : 1))
										{
											priority = (turn == player_turn ? 2 : 1);
											index = i - 22;
											break;
										}
									}
								}

								if (pos_list[(long long)i + 88]->bClick == CLICK_NONE)
								{
									if (priority > (turn == player_turn ? 2 : 1))
									{
										priority = (turn == player_turn ? 2 : 1);
										index = i + 88;
										break;
									}
								}
							}
							else
							{
								if (i - 22 >= 0)
								{
									INT left_turn = pos_list[(long long)i - 22]->bClick;
									INT right_turn = pos_list[(long long)i + 66]->bClick;

									if (left_turn == CLICK_NONE && right_turn == CLICK_NONE)
									{
										//3. 상대 양쪽 안막힌 3개를 막는다.
										// ㅁ -> xㅁㅁㅁx

										//5. 내 양쪽 안막힌 3개를 잇는다.
										// ■ -> x■■■x

										if (priority > (turn != player_turn) ? 3 : 5)
										{
											priority = (turn != player_turn) ? 3 : 5;
											index = (turn != player_turn) ? (i - 22) : (i + 66);
											break;
										}
									}
									else if (left_turn == CLICK_NONE && right_turn != CLICK_NONE)
									{
										//4. 상대 한쪽 안막힌 3개를 막는다.
										// ㅁ -> xㅁㅁㅁ■

										//6. 내 한쪽 안막힌 3개를 잇는다.
										// ■ -> x■■■ㅁ

										if (priority > (turn != player_turn) ? 4 : 6)
										{
											priority = (turn != player_turn) ? 4 : 6;
											index = i - 22;
											break;
										}
									}
									else if (left_turn == player_turn && right_turn == CLICK_NONE)
									{
										//4. 상대 한쪽 안막힌 3개를 막는다.
										// ㅁ -> ■ㅁㅁㅁx

										//6. 내 한쪽 안막힌 3개를 잇는다.
										// ■ -> ㅁ■■■x

										if (priority > (turn != player_turn) ? 4 : 6)
										{
											priority = (turn != player_turn) ? 4 : 6;
											index = i + 66;
											break;
										}
									}
								}
							}
						}
						else if (turn == player_turn && pos_list[(long long)i + 44]->bClick == CLICK_NONE)
						{
							if (priority > 7)
							{
								priority = 7;
								index = i + 44;
								break;
							}
						}
					}
					else if (pos_list[(long long)i + 22]->bClick == CLICK_NONE)
					{
						if (priority >= 8)
						{
							priority = 8;
							index_list.insert(i + 22);
							break;
						}
					}
				}
			}
		}

		//좌측 하단
		int lb = i + 20;
		int lb2 = i - 80;
		if (lb2 >= 0 && pos_list.size() > lb && pos_list[i]->x >= 145.f && pos_list[i]->y < 610.f)
		{
			for (int turn = 0; turn < player_size; turn++)
			{
				if (pos_list[i]->bClick == turn)
				{
					if (pos_list[(long long)i - 20]->bClick == turn)
					{
						if (pos_list[(long long)i - 40]->bClick == turn)
						{
							if (pos_list[(long long)i - 60]->bClick == turn)
							{
								if (pos_list[(long long)i + 20]->bClick == CLICK_NONE)
								{
									if (priority > (turn == player_turn ? 2 : 1))
									{
										priority = (turn == player_turn ? 2 : 1);
										index = i + 20;
										break;
									}
								}

								if (pos_list[(long long)i - 80]->bClick == CLICK_NONE)
								{
									if (priority > (turn == player_turn ? 2 : 1))
									{
										priority = (turn == player_turn ? 2 : 1);
										index = i - 80;
										break;
									}
								}
							}
							else
							{
								INT left_turn = pos_list[(long long)i + 20]->bClick;
								INT right_turn = pos_list[(long long)i - 60]->bClick;

								if (left_turn == CLICK_NONE && right_turn == CLICK_NONE)
								{
									//3. 상대 양쪽 안막힌 3개를 막는다.
									// ㅁ -> xㅁㅁㅁx

									//5. 내 양쪽 안막힌 3개를 잇는다.
									// ■ -> x■■■x

									if (priority > (turn != player_turn) ? 3 : 5)
									{
										priority = (turn != player_turn) ? 3 : 5;
										index = (turn != player_turn) ? (i + 20) : (i - 60);
										break;
									}
								}
								else if (left_turn == CLICK_NONE && right_turn != CLICK_NONE)
								{
									//4. 상대 한쪽 안막힌 3개를 막는다.
									// ㅁ -> xㅁㅁㅁ■

									//6. 내 한쪽 안막힌 3개를 잇는다.
									// ■ -> x■■■ㅁ

									if (priority > (turn != player_turn) ? 4 : 6)
									{
										priority = (turn != player_turn) ? 4 : 6;
										index = i + 20;
										break;
									}
								}
								else if (left_turn == player_turn && right_turn == CLICK_NONE)
								{
									//4. 상대 한쪽 안막힌 3개를 막는다.
									// ㅁ -> ■ㅁㅁㅁx

									//6. 내 한쪽 안막힌 3개를 잇는다.
									// ■ -> ㅁ■■■x

									if (priority > (turn != player_turn) ? 4 : 6)
									{
										priority = (turn != player_turn) ? 4 : 6;
										index = i - 60;
										break;
									}
								}
							}
						}
						else if (turn == player_turn && pos_list[(long long)i - 40]->bClick == CLICK_NONE)
						{
							if (priority > 7)
							{
								priority = 7;
								index = i - 40;
								break;
							}
						}
					}
					else if (pos_list[(long long)i - 20]->bClick == CLICK_NONE)
					{
						if (priority >= 8)
						{
							priority = 8;
							index_list.insert(i - 20);
							break;
						}
					}
				}
			}
		}
	}



	if (index >= 0)
	{
		std::cout << "index : " << index << '\n';
		pos_list[index]->bClick = player_turn;
	}
	else if (index_list.size() > 0)
	{
		std::cout << "index_list : " << index_list.size() << '\n';

		UINT rand;
		rand_s(&rand);

		auto iter = next(index_list.begin(), rand % index_list.size());

		pos_list[*iter]->bClick = player_turn;

		std::cout << "index : " << *iter << '\n';
	}
	else
	{
		//9. 아무 데나 놓는다.
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
		//가로
		int x_ = i + 84;
		if (pos_list.size() > x_ && pos_list[i]->x <= 610.f)
		{
			if (pos_list[i]->bClick == player
				&& pos_list[(long long)i + 21]->bClick == player
				&& pos_list[(long long)i + 42]->bClick == player
				&& pos_list[(long long)i + 63]->bClick == player
				&& pos_list[(long long)i + 84]->bClick == player)
			{
				return TRUE;
			}
		}

		//세로
		int y_ = i + 4;
		if (pos_list.size() > y_ && pos_list[i]->y < 610.f)
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

		//우측 하단
		int rb = i + 88;
		if (pos_list.size() > rb && pos_list[i]->x <= 610.f && pos_list[i]->y < 610.f)
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

		//좌측 하단
		int lb = i - 80;
		if (pos_list.size() > lb && pos_list[i]->x >= 145.f && pos_list[i]->y < 610.f)
		{
			if (pos_list[i]->bClick == player
				&& pos_list[(long long)i - 20]->bClick == player
				&& pos_list[(long long)i - 40]->bClick == player
				&& pos_list[(long long)i - 60]->bClick == player
				&& pos_list[(long long)i - 80]->bClick == player)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}