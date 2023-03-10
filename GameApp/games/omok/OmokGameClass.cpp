#include "stdafx.h"
#include "OmokGameClass.h"
#include <system/SystemClass.h>

OmokGameClass::OmokGameClass(UINT player_size)
{
	//採乞 持失切拭辞 拝 依級 皐壱 馬奄

	//1. 置企帖左陥 旋聖 井酔 姐呪 匝戚奄. (据朝球澗 背雁 紫牌 蒸走幻 析舘 因搭 坦軒)
	this->player_size = player_size;

	this->player_vector.resize(player_size);

	//2. 郊杵宜 是帖 走舛
	float len = 750.f;
	for (float x = 15.f; x <= len; x += 35)
	{
		for (float y = 15.f; y < len; y += 35)
		{
			pos_list.push_back(new OmokPos{x, y});
		}
	}

	//3. 獣拙 巴傾戚嬢 仙竺舛
	unsigned int rand;
	rand_s(&rand);

	player_turn = rand % player_size;

	//4. 惟績 展戚袴 室特

	DWORD threadId = 0;
	DWORD threadParam = 0;
	CreateThread(nullptr, 0, SystemClass::GameMainThread, &threadParam, 0, &threadId);
}

OmokGameClass::~OmokGameClass()
{
	for (auto iter = pos_list.begin(); iter != pos_list.end(); iter++)
	{
		delete *iter;
	}

	pos_list.clear();
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
	//走管 蓄亜
	//酔識授是

	int index = -1;
	int priority = 9;
	std::set<int> index_list;

	for (int i = 0; i < pos_list.size(); i++)
	{
		//亜稽 伊事
		int x_ = i + 84;
		if (pos_list.size() > x_ && pos_list[i]->x <= 610.f)
		{
			//丞 巴傾戚嬢 疑獣 伊事 亜管馬糠稽 疑獣 伊事
			for (int turn = 0; turn < player_size; turn++)
			{
				//唖 渡税 郊杵硝 伊事
				if (pos_list[i]->bClick == turn)
				{
					//陥製 是帖亀 照厳縛澗走 端滴
					if (pos_list[(long long)i + 21]->bClick == turn)
					{
						//3腰属 是帖亀 照厳縛澗走 端滴
						if (pos_list[(long long)i + 42]->bClick == turn)
						{
							//4腰属 是帖亀 照厳縛澗走 端滴
							if (pos_list[(long long)i + 63]->bClick == turn)
							{
								//1. 雌企税 4鯵研 巷繕闇 厳澗陥.
								//2. 鎧 4鯵研 巷繕闇 責澗陥.
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
								//蟹 : ＝, 雌企 : け

								if (i - 21 >= 0)
								{
									INT left_turn = pos_list[(long long)i - 21]->bClick;
									INT right_turn = pos_list[(long long)i + 63]->bClick;

									if (left_turn == CLICK_NONE && right_turn == CLICK_NONE)
									{
										//3. 雌企 丞楕 照厳微 3鯵研 厳澗陥.
										// け -> xけけけx

										//5. 鎧 丞楕 照厳微 3鯵研 責澗陥.
										// ＝ -> x＝＝＝x

										if (priority > (turn != player_turn) ? 3 : 5)
										{
											priority = (turn != player_turn) ? 3 : 5;
											index = (turn != player_turn) ? (i - 21) : (i + 63);
											break;
										}
									}
									else if (left_turn == CLICK_NONE && right_turn != CLICK_NONE)
									{
										//4. 雌企 廃楕 照厳微 3鯵研 厳澗陥.
										// け -> xけけけ＝

										//6. 鎧 廃楕 照厳微 3鯵研 責澗陥.
										// ＝ -> x＝＝＝け

										if (priority > (turn != player_turn) ? 4 : 6)
										{
											priority = (turn != player_turn) ? 4 : 6;
											index = i - 21;
											break;
										}
									}
									else if (left_turn == player_turn && right_turn == CLICK_NONE)
									{
										//4. 雌企 廃楕 照厳微 3鯵研 厳澗陥.
										// け -> ＝けけけx

										//6. 鎧 廃楕 照厳微 3鯵研 責澗陥.
										// ＝ -> け＝＝＝x

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
							//7. 鎧 丞楕 照厳微 2鯵研 責澗陥.
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
						//8. 鎧 宜 新拭 責澗陥.
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

		//室稽 伊事
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
										//3. 雌企 丞楕 照厳微 3鯵研 厳澗陥.
										// け -> xけけけx

										//5. 鎧 丞楕 照厳微 3鯵研 責澗陥.
										// ＝ -> x＝＝＝x

										if (priority > (turn != player_turn) ? 3 : 5)
										{
											priority = (turn != player_turn) ? 3 : 5;
											index = (turn != player_turn) ? (i - 1) : (i + 3);
											break;
										}
									}
									else if (left_turn == CLICK_NONE && right_turn != CLICK_NONE)
									{
										//4. 雌企 廃楕 照厳微 3鯵研 厳澗陥.
										// け -> xけけけ＝

										//6. 鎧 廃楕 照厳微 3鯵研 責澗陥.
										// ＝ -> x＝＝＝け

										if (priority > (turn != player_turn) ? 4 : 6)
										{
											priority = (turn != player_turn) ? 4 : 6;
											index = i - 1;
											break;
										}
									}
									else if (left_turn == player_turn && right_turn == CLICK_NONE)
									{
										//4. 雌企 廃楕 照厳微 3鯵研 厳澗陥.
										// け -> ＝けけけx

										//6. 鎧 廃楕 照厳微 3鯵研 責澗陥.
										// ＝ -> け＝＝＝x

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

		//酔著 馬舘
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
										//3. 雌企 丞楕 照厳微 3鯵研 厳澗陥.
										// け -> xけけけx

										//5. 鎧 丞楕 照厳微 3鯵研 責澗陥.
										// ＝ -> x＝＝＝x

										if (priority > (turn != player_turn) ? 3 : 5)
										{
											priority = (turn != player_turn) ? 3 : 5;
											index = (turn != player_turn) ? (i - 22) : (i + 66);
											break;
										}
									}
									else if (left_turn == CLICK_NONE && right_turn != CLICK_NONE)
									{
										//4. 雌企 廃楕 照厳微 3鯵研 厳澗陥.
										// け -> xけけけ＝

										//6. 鎧 廃楕 照厳微 3鯵研 責澗陥.
										// ＝ -> x＝＝＝け

										if (priority > (turn != player_turn) ? 4 : 6)
										{
											priority = (turn != player_turn) ? 4 : 6;
											index = i - 22;
											break;
										}
									}
									else if (left_turn == player_turn && right_turn == CLICK_NONE)
									{
										//4. 雌企 廃楕 照厳微 3鯵研 厳澗陥.
										// け -> ＝けけけx

										//6. 鎧 廃楕 照厳微 3鯵研 責澗陥.
										// ＝ -> け＝＝＝x

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

		//疎著 馬舘
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
									//3. 雌企 丞楕 照厳微 3鯵研 厳澗陥.
									// け -> xけけけx

									//5. 鎧 丞楕 照厳微 3鯵研 責澗陥.
									// ＝ -> x＝＝＝x

									if (priority > (turn != player_turn) ? 3 : 5)
									{
										priority = (turn != player_turn) ? 3 : 5;
										index = (turn != player_turn) ? (i + 20) : (i - 60);
										break;
									}
								}
								else if (left_turn == CLICK_NONE && right_turn != CLICK_NONE)
								{
									//4. 雌企 廃楕 照厳微 3鯵研 厳澗陥.
									// け -> xけけけ＝

									//6. 鎧 廃楕 照厳微 3鯵研 責澗陥.
									// ＝ -> x＝＝＝け

									if (priority > (turn != player_turn) ? 4 : 6)
									{
										priority = (turn != player_turn) ? 4 : 6;
										index = i + 20;
										break;
									}
								}
								else if (left_turn == player_turn && right_turn == CLICK_NONE)
								{
									//4. 雌企 廃楕 照厳微 3鯵研 厳澗陥.
									// け -> ＝けけけx

									//6. 鎧 廃楕 照厳微 3鯵研 責澗陥.
									// ＝ -> け＝＝＝x

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
		//9. 焼巷 汽蟹 兜澗陥.
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
	//亜稽, 室稽, 神献楕 焼掘, 図楕 焼掘 端滴
	for (int i = 0; i < pos_list.size(); i++)
	{
		//亜稽
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

		//室稽
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

		//酔著 馬舘
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

		//疎著 馬舘
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