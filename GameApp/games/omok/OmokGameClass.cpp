#include "stdafx.h"
#include "OmokGameClass.h"
#include <system/SystemClass.h>

OmokGameClass::OmokGameClass(UINT player_size)
{
	//�θ� �����ڿ��� �� �͵� ���� �ϱ�

	//1. �ִ�ġ���� ���� ��� ���� ���̱�. (��ī��� �ش� ���� ������ �ϴ� ���� ó��)
	this->player_size = player_size;

	this->player_vector.resize(player_size);

	//2. �ٵϵ� ��ġ ����
	float len = 750.f;
	for (float x = 15.f; x <= len; x += 35)
	{
		for (float y = 15.f; y <= len; y += 35)
		{
			pos_list.push_back(new OmokPos{x, y});
		}
	}

	//3. ���� �÷��̾� �缳��

	player_turn = rand() % player_size;

	//4. ���� Ÿ�̸� ����

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
	//���� �߰�
	//�켱����

	pos_list[100]->bClick = player_turn;
	pos_list[100 + 23]->bClick = player_turn;
	pos_list[100 + 46]->bClick = player_turn;

	int index = -1;

	for (int i = 0; i < pos_list.size(); i++)
	{
		//���� ��ġ ã������ �׸� �ϱ�.
		if (index >= 0)
		{
			break;
		}


		int x_ = i + 88;
		if (pos_list.size() > x_)
		{
			//�� �÷��̾� ���� �˻� �����ϹǷ� ���� �˻�
			for (int turn = 0; turn < player_size; turn++)
			{
				//��ġ�� �����̻� �ȸ��� 3���� �ִ��� �˻�
				if (pos_list[i]->bClick == turn
					&& pos_list[(long long)i + 22]->bClick == turn
					&& pos_list[(long long)i + 44]->bClick == turn
					)
				{

					//4��° ��ġ�� �ȸ������� üũ
					if (pos_list[(long long)i + 66]->bClick == turn)
					{
						//1. 4������ 5���� �����ϸ� ���� ���� �Ѵ�.
						//2. ����� 4���� ������ ���´�.
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
						//3. �� ���� �ȸ��� 3���� �մ´�.
						//4. �� ���� �ȸ��� 3���� �մ´�.

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

	//5. ��� ���� �ȸ��� 3���� ���´�.
	//6. ��� ���� �ȸ��� 3���� ���´�.
	//7. �� ���� �ȸ��� 2���� �մ´�.
	//8. �� �� ���� ���´�.

	std::cout << "index : " << index << '\n';

	if (index >= 0)
	{
		pos_list[index]->bClick = player_turn;
	}
	else
	{
		//9. �ƹ����� ���´�.
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
	//����, ����, ������ �Ʒ�, ���� �Ʒ� üũ
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