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

	//4. ���� Ÿ�̸� ����

	DWORD threadId = 0;
	DWORD threadParam = 0;
	CreateThread(nullptr, 0, SystemClass::GameMainThread, &threadParam, 0, &threadId);
}

Card* CardGameClass::GetNextCard()
{
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
	return FALSE;
}