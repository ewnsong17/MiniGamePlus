#include "stdafx.h"
#include "CardGameClass.h"
#include <system/SystemClass.h>

CardGameClass::CardGameClass(UINT player_size)
{
	//부모 생성자에서 할 것들 빼고 하기

	//1. 최대치보다 적을 경우 갯수 줄이기. (원카드는 해당 사항 없지만 일단 공통 처리)
	this->player_size = player_size;

	this->player_vector.resize(player_size);

	//2. 원카드 기본 카드 값 세팅

	for (int color = 0; color < 4; color++)
	{
		for (int num = 1; num <= 13; num++)
		{
			card_list.push_back(new Card(color, num));
		}
	}

	std::random_shuffle(card_list.begin(), card_list.end());

	//3. 각 플레이어에게 카드 7장씩 배분

	for (int i = 0; i < 28; i++)
	{
		card_list[i]->owner = (INT) (i / 7);
	}

	//4. 게임 타이머 세팅

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
		//같은 번호일 경우 무조건 OK
		if (next_card->number == card->number)
		{
			return TRUE;
		}

		//같은 색일 경우도 OK
		if (next_card->color == card->color)
		{
			return TRUE;
		}
	}
	return FALSE;
}