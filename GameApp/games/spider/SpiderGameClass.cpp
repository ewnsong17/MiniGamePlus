#include <stdafx.h>
#include <spider/SpiderGameClass.h>

SpiderGameClass::SpiderGameClass(UINT player_size)
{
	//�θ� �����ڿ��� �� �͵� ���� �ϱ�

	//1. �ִ�ġ���� ���� ��� ���� ���̱�. (��ī��� �ش� ���� ������ �ϴ� ���� ó��)
	this->player_size = player_size;

	this->player_vector.resize(player_size);

	//2. ī�� ��ġ ����
	int num = 0;
	for (int i = 0; i < 10; i++)
	{
		card_list.push_back(new SpiderCardList(33.f + i * 95, 15.f));

		for (int j = (i < 4 ? 6 : 5); j > 0; j--)
		{
			SpiderCard* card = new SpiderCard(num % 13 + 1);
			num++;
			card_list[i]->card_vec.push_back(card);
		}

		std::random_shuffle(card_list[i]->card_vec.begin(), card_list[i]->card_vec.end());

		card_list[i]->card_vec[card_list[i]->card_vec.size() - 1]->bShow = TRUE;
	}

	for (int i = 54; i < 104; i++)
	{
		card_grave_list.push_back(new SpiderCard(i % 13 + 1));
	}

	std::random_shuffle(card_grave_list.begin(), card_grave_list.end());


	//3. ���� �÷��̾� �缳��
	player_turn = 0;
}

SpiderGameClass::~SpiderGameClass()
{
	for (int i = 0; i < card_list.size(); i++)
	{
		for (auto iter = card_list[i]->card_vec.begin(); iter != card_list[i]->card_vec.end(); iter++)
		{
			delete *iter;
		}

		card_list[i]->card_vec.clear();

		delete card_list[i];
	}

	card_list.clear();

	for (int i = 0; i < card_grave_list.size(); i++)
	{
		delete card_grave_list[i];
	}

	card_grave_list.clear();
}

std::wstring SpiderGameClass::GetCardImage(SpiderCard* card)
{
	std::wstring num_str;

	switch (card->number)
	{
	case 1:
		num_str = 'A';
		break;
	case 11:
		num_str = 'J';
		break;
	case 12:
		num_str = 'Q';
		break;
	case 13:
		num_str = 'K';
		break;
	default:
		num_str = std::to_wstring(card->number);
		break;
	}

	std::wstring w_str = L"image\\Card\\Spade\\" + num_str + L".jpg";

	//	std::wcout << w_str << '\n';

	return w_str;
}

VOID SpiderGameClass::CompleteSet()
{
	for (int i = 0; i < card_list.size(); i++)
	{
		std::vector<SpiderCard*>* card_vec = &(card_list[i]->card_vec);

		INT last_card = 14;

		//ī�庰�� K-A���� ��Ʈ������ �ִ��� ����
		for (int j = 0; j < card_vec->size(); j++)
		{
			if (last_card - card_vec->at(j)->number == 1)
			{
				last_card--;
			}
			else
			{
				last_card = 14;
			}
		}

		std::cout << "last : " << last_card << '\n';

		if (last_card == 1)
		{
			for (auto iter = card_vec->end() - 13; iter != card_vec->end(); iter++)
			{
				delete *iter;
			}

			card_vec->resize(card_vec->size() - 13);

			//�������� �� ī�带 Show�ǰ� ����
			if (card_vec->size() > 0)
			{
				card_vec->at(card_vec->size() - 1)->bShow = TRUE;
			}
		}
	}
}

VOID SpiderGameClass::GetCardFromGraves(HWND hWnd)
{
	if (grave_cnt > 0)
	{
		grave_cnt--;

		std::vector<SpiderCard*> add_vec(card_grave_list.end() - 10, card_grave_list.end());

		for (int i = 0; i < card_list.size(); i++)
		{
			add_vec[i]->bShow = TRUE;
			card_list[i]->card_vec.push_back(add_vec[i]);
		}

		card_grave_list.resize(card_grave_list.size() - 10);

		InvalidateRect(hWnd, nullptr, TRUE);
	}
}

VOID SpiderGameClass::MoveCard(SpiderCardList* moveTo)
{
	if (select_index >= 0 && select_index_2 >= 0)
	{
		SpiderCardList* moveFrom = card_list[select_index];

		//���ڰ� 1 ���̿��� ���� ����
		if (moveTo->card_vec.size() == 0 || (moveTo->card_vec[moveTo->card_vec.size() - 1]->number - moveFrom->card_vec[select_index_2]->number == 1))
		{
			//���� �ű�� ���� ���̰�..
			int push = 0;
			for (int i = select_index_2; i < moveFrom->card_vec.size(); i++)
			{
				push++;
				moveTo->card_vec.push_back(moveFrom->card_vec[i]);
			}

			//���µǾ��ִ� ī����� ���� ����
			moveFrom->card_vec.resize(moveFrom->card_vec.size() - push);

			if (moveFrom->card_vec.size() > 0)
			{
				//�������� �� ī�带 Show�ǰ� ����
				moveFrom->card_vec[moveFrom->card_vec.size() - 1]->bShow = TRUE;
			}

			CompleteSet();
		}
	}
}

VOID SpiderGameClass::GetMouseClick(HWND hWnd, INT xPos, INT yPos)
{
	D2D1_RECT_F grave_rect = D2D1::RectF(875.f, 550.f + grave_cnt, 875.f + (0.8f * CARD_WIDTH), 550.f + grave_cnt + (0.8f * CARD_HEIGHT));

	//���� Ŭ�� �� ī�� ��Ʈ ���� �̱�
	if (xPos >= grave_rect.left && xPos <= grave_rect.right)
	{
		if (yPos >= grave_rect.top && yPos <= grave_rect.bottom)
		{
			if (grave_cnt > 0)
			{
				GetCardFromGraves(hWnd);
				return;
			}
		}
	}

	int index = -1;
	int index_2 = -1;

	//���� �� ī�� ��ü�� �������.
	for (int i = 0; i < card_list.size(); i++)
	{
		if (xPos >= card_list[i]->x && xPos <= (card_list[i]->x + (0.8f * CARD_WIDTH)))
		{
			index = i;

			std::vector<SpiderCard*> card_vec = card_list[i]->card_vec;

			for (int j = card_vec.size() - 1; j >= 0; j--)
			{
				if (yPos >= (card_list[i]->y + (18.5f * j)) && yPos <= (card_list[i]->y + (18.5f * j) + (0.8f * CARD_HEIGHT)))
				{
					if (card_vec[j]->bShow)
					{
						index_2 = j;
						break;
					}
				}
			}

			if (index_2 >= 0)
			{
				int click_num = card_vec[index_2]->number;
				int j = index_2;
				std::cout << "clicked : " << click_num << '\n';
				std::cout << "click_index : " << index << '\n';

				for (; j < card_vec.size(); j++)
				{
					if (click_num == card_vec[j]->number)
					{
						click_num--;
					}
					else
					{
						std::cout << "index diff : " << click_num << " : " << card_vec[j]->number << '\n';
						std::cout << "index update : " << j << '\n';
						index_2 = j;
						click_num = card_vec[j]->number - 1;
					}
				}
			}
		}
	}

	//�̹� ���õ� ī�尡 �ִٸ�
	if (select_index >= 0 && select_index_2 >= 0)
	{
		//�ٸ� ī�� Ŭ�� �� �̵� ������ ��� �̵�
		if (index >= 0)
		{
			if (select_index != index)
			{
				MoveCard(card_list[index]);
			}
		}
		

		select_index = -1;
		select_index_2 = -1;
	}
	else
	{
		//���õ� ī�尡 ���� �� ī�� ���� �� �� ī�带 ���õ� ī��� ����
		if (index >= 0 && index_2 >= 0)
		{
			select_index = index;
			select_index_2 = index_2;
		}
	}

	std::cout << "index : " << index << '\n';
	std::cout << "index_2 : " << index_2 << '\n';
	std::cout << '\n';

	InvalidateRect(hWnd, nullptr, TRUE);
}