#include "stdafx.h"
#include "CardGameClass.h"
#include <system/SystemClass.h>

CardGameClass::CardGameClass(UINT player_size)
{
	//�θ� �����ڿ��� �� �͵� ���� �ϱ�

	//1. �ִ�ġ���� ���� ��� ���� ���̱�. (��ī��� �ش� ���� ������ �ϴ� ���� ó��)
	this->player_size = player_size;

	this->player_vector.resize(player_size);

	//2. ��ī�� �⺻ ī�� �� ����

	for (int color = 0; color < 4; color++)
	{
		for (int num = 1; num <= 13; num++)
		{
			card_list.push_back(new Card(color, num));
		}
	}

	std::random_shuffle(card_list.begin(), card_list.end());

	//3. �� �÷��̾�� ī�� 7�徿 ���

	for (int i = 0; i < 28; i++)
	{
		card_list[i]->owner = (INT) (i / 7);
	}

	//ī�� �ϳ��� �������� ������ �÷�����
	card_list[28]->owner = CARD_DECK;

	//4. ���� Ÿ�̸� ����

	DWORD threadId = 0;
	DWORD threadParam = 0;
	CreateThread(nullptr, 0, SystemClass::GameMainThread, &threadParam, 0, &threadId);
}

Card* CardGameClass::GetNextCard()
{
	if (FAKE_CARD != nullptr)
	{
		return FAKE_CARD;
	}

	for (int i = 0; i < card_list.size(); i++)
	{
		if (card_list[i]->owner == CARD_DECK)
		{
			return card_list[i];
		}
	}
	return nullptr;
}

std::vector<Card*> CardGameClass::GetPlayerCards(int index)
{
	std::vector<Card*> card_vec;

	for (int i = 0; i < card_list.size(); i++)
	{
		if (card_list[i]->owner == index)
		{
			card_vec.push_back(card_list[i]);
		}
	}
	return card_vec;
}

std::wstring CardGameClass::GetCardImage(Card* card)
{
	std::wstring color_str;
	std::wstring num_str;
	switch (card->color)
	{
		case 0:
			color_str = L"Clover";
			break;
		case 1:
			color_str = L"Diamond";
			break;
		case 2:
			color_str = L"Heart";
			break;
		case 3:
			color_str = L"Spade";
			break;
	}

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

	std::wstring w_str = L"image\\Card\\" + color_str + L"\\" + num_str + L".jpg";

//	std::wcout << w_str << '\n';

	return w_str;
}

BOOL CardGameClass::IsAllowToUse(Card* card)
{
	Card* next_card = GetNextCard();

	if (next_card != nullptr)
	{
		if (ATTACK_CNT > 0)
		{
			//���� ������ ���

			//���� or ����ī�常 ����
			switch (next_card->number)
			{
				case 1:
				{
					//���� A�� ����
					if (card->number == 1)
					{
						return TRUE;
					}
					break;
				}
				case 2:
				{
					//���� 2 Ȥ�� ���� ������ A�� ����
					if (card->number == 2 || (card->number == 1 && card->color == next_card->color))
					{
						return TRUE;
					}
					break;
				}
				case 3:
				{
					//���� ī��� ���� ���� ��� OK
					if (next_card->color == card->color)
					{
						return TRUE;
					}
					break;
				}
			}
		}
		else
		{
			//���� ���°� �ƴ� ���
			
			//���� ��ȣ�� ��� ������ OK
			if (next_card->number == card->number)
			{
				return TRUE;
			}

			//���� ���� ��쵵 OK
			if (next_card->color == card->color)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

VOID CardGameClass::PlayerDead(int playerID)
{
	//��� �� ������ �ִ� ī�� ���� ������ ������
	std::vector<Card*> card_list = GetPlayerCards(playerID);

	for (int i = 0; i < card_list.size(); i++)
	{
		card_list[i]->owner = CARD_GRAVE;
	}

	player_vector[playerID].player_alive = FALSE;
}

VOID CardGameClass::GetCardFromGraves(int ownerID)
{
	std::vector<Card*> card_graves;

	for (int i = 0; i < card_list.size(); i++)
	{
		if (card_list[i]->owner == CARD_GRAVE)
		{
			card_graves.push_back(card_list[i]);
		}
	}

	special_text = L"";

//	std::cout << "click grave " << card_graves.size()  << '\n';

	if (card_graves.size() > 0 && !SELECT_COLOR)
	{
		std::random_shuffle(card_graves.begin(), card_graves.end());


		if (ATTACK_CNT > 0)
		{
			//TODO::��� ó��
			if (card_graves.size() < ATTACK_CNT || GetPlayerCards(ownerID).size() + ATTACK_CNT > DEAD_LINE)
			{
				PlayerDead(ownerID);
				ATTACK_CNT = 0;
				return;
			}

			for (int i = 0; i < ATTACK_CNT; i++)
			{
				card_graves[i]->owner = ownerID;
			}

			ATTACK_CNT = 0;
		}
		else
		{
			if (card_graves.size() <= 0 || GetPlayerCards(ownerID).size() + 1 >= DEAD_LINE)
			{
				PlayerDead(ownerID);
				return;
			}

			card_graves[0]->owner = ownerID;
		}

//		std::cout << "GET : " << card_graves[0]->number << '\n';
	}
}

VOID CardGameClass::SetGameEnd()
{
	timer = -10000;
}

VOID CardGameClass::ShowCardEffect(Card* selectCard)
{

	switch (selectCard->number)
	{
		case 1:
			ATTACK_CNT += selectCard->number == 3 ? 5 : 3; // �����̵� ���̽�
			special_text = L"����! (" + std::to_wstring(ATTACK_CNT) + L")";
			break;
		case 2:
			ATTACK_CNT += 2;
			special_text = L"����! (" + std::to_wstring(ATTACK_CNT) + L")";
			break;
		case 3:
			if (ATTACK_CNT > 0)
			{
				special_text = L"����!";
			}
			break;
		case 7:
			if (player_turn == 0)
			{
				SELECT_COLOR = TRUE;
				SystemClass::ShowColorButtons(TRUE);
			}
			else
			{
				unsigned int rand;
				rand_s(&rand);

				FAKE_CARD = new Card(rand % 4, 7);
			}
			break;
		case 11:
			TURN_JUMP = TRUE;
			special_text = L"����!";
			break;
		case 12:
			TURN_REVERSE = !TURN_REVERSE;
			special_text = L"�ǵ�����!";
			break;
		case 13:
			BLOCK_TURN_SET = TRUE;
			special_text = L"�� �� ��!";
			break;
		default:
			special_text = L"";
			break;
	}
}

VOID CardGameClass::SetNextTurn(HWND hWnd)
{
	//���� ���� ���� ���� üũ
	if (player_vector[player_turn].player_alive && GetPlayerCards(player_turn).size() <= 0)
	{
		SetGameEnd();
		return;
	}

	int alive_cnt = 0;
	for (int i = 0; i < player_size; i++)
	{
		if (player_vector[i].player_alive)
		{
			alive_cnt++;
		}
	}

	if (alive_cnt <= 1)
	{
		SetGameEnd();
		return;
	}

	//�� �ٲٱ�
	if (!BLOCK_TURN_SET && !SELECT_COLOR)
	{
		for (int i = 0; i < (TURN_JUMP ? 2 : 1);)
		{
			if (TURN_REVERSE)
			{
				player_turn--;

				if (player_turn < 0)
				{
					player_turn = player_size - 1;
				}
			}
			else
			{
				player_turn++;

				if (player_turn >= player_size)
				{
					player_turn = 0;
				}
			}

			if (player_vector[player_turn].player_alive)
			{
				i++;
			}
		}
	}

	BLOCK_TURN_SET = FALSE;
	TURN_JUMP = FALSE;

//	std::cout << "next turn : " << player_turn << '\n';

	//Ÿ�̸� �缳��
	timer = 30;

	InvalidateRect(hWnd, nullptr, TRUE);
}

VOID CardGameClass::GetMouseClick(HWND hWnd, INT xPos, INT yPos)
{
	if (timer > 0 && player_turn == 0 && !SELECT_COLOR)
	{

		//���� Ŭ�� �� ī�� �ϳ� �̱�
		if (xPos >= grave_rect.left && xPos <= grave_rect.right)
		{
			if (yPos >= grave_rect.top && yPos <= grave_rect.bottom)
			{
				GetCardFromGraves(0);
				SetNextTurn(hWnd);
				return;
			}
		}

		std::vector<Card*> myCards = GetPlayerCards(0);

		//		std::cout << "myPos : [" << xPos << "|" << yPos << "]" << '\n';

		Card* selectCard = nullptr;
		int index = -1;

		for (int i = myCards.size() - 1; i >= 0; i--)
		{
			if (xPos >= myCards[i]->rect.left && xPos <= myCards[i]->rect.right)
			{
				if (yPos >= myCards[i]->rect.top && yPos <= myCards[i]->rect.bottom)
				{
					selectCard = myCards[i];
					index = i;
					break;
				}
			}
		}

		if (selectCard != nullptr)
		{
			//ī�� ���Ⱑ ������ ���
			//1. ī�� �����
			//2. �� �ٲٰ�
			//3. Ÿ�̸� �ٽ� �׸��鼭
			//4. ȭ�� ���ε�
			if (IsAllowToUse(selectCard))
			{

				//����ũ ī�� �����
				if (FAKE_CARD != nullptr)
				{
					FAKE_CARD = nullptr;
				}

				//ī�� �����
				GetNextCard()->owner = CARD_GRAVE;
				selectCard->owner = CARD_DECK;

				//ī�� ȿ�� �ߵ� �� ���� ����
				ShowCardEffect(selectCard);

				//�� �����ϱ�
				SetNextTurn(hWnd);
			}
		}
	}
}

VOID CardGameClass::TurnCPU(HWND hWnd)
{
	if (timer > 0)
	{
		Sleep(500);

		std::vector<Card*> cpu_cards = GetPlayerCards(player_turn);
		Card* select_card;
		for (int i = 0; i < cpu_cards.size(); i++)
		{
			select_card = cpu_cards[i];
			if (IsAllowToUse(select_card))
			{
				//����ũ ī�� �����
				if (FAKE_CARD != nullptr)
				{
					FAKE_CARD = nullptr;
				}

				//ī�� �����
				GetNextCard()->owner = CARD_GRAVE;
				select_card->owner = CARD_DECK;

				//ī�� ȿ�� �ߵ� �� ���� ����
				ShowCardEffect(select_card);

				//�� �����ϱ�
				SetNextTurn(hWnd);
				return;
			}
		}

		GetCardFromGraves(player_turn);
		SetNextTurn(hWnd);
	}
}

VOID CardGameClass::SetColor(HWND hWnd, DWORD color)
{
	int type = -1;
	switch (color)
	{
		case IDC_CARD_CLOVER:
			type = 0;
			break;
		case IDC_CARD_DIAMOND:
			type = 1;
			break;
		case IDC_CARD_HEART:
			type = 2;
			break;
		case IDC_CARD_SPADE:
			type = 3;
			break;
	}

	FAKE_CARD = new Card(type, 7);
	FAKE_CARD->owner = CARD_DECK;
	SELECT_COLOR = FALSE;

	SetNextTurn(hWnd);
}