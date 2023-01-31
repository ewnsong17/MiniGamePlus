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

	//카드 하나는 앞장으로 뒤집어 올려놓기
	card_list[28]->owner = CARD_DECK;

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

//	std::cout << "click grave " << card_graves.size()  << '\n';

	if (card_graves.size() > 0)
	{
		std::random_shuffle(card_graves.begin(), card_graves.end());

		card_graves[0]->owner = ownerID;

//		std::cout << "GET : " << card_graves[0]->number << '\n';
	}
}

VOID CardGameClass::SetGameEnd(HWND hWnd)
{
	timer = 0;
}

VOID CardGameClass::SetNextTurn(HWND hWnd)
{
	//게임 종료 가능 여부 체크
	if (GetPlayerCards(player_turn).size() <= 0)
	{
		SetGameEnd(hWnd);
		return;
	}

	//턴 바꾸기
	player_turn++;

	if (player_turn >= player_size)
	{
		player_turn = 0;
	}

//	std::cout << "next turn : " << player_turn << '\n';

	//타이머 재설정
	timer = 30;

	InvalidateRect(hWnd, nullptr, TRUE);
}

VOID CardGameClass::GetMouseClick(HWND hWnd, INT xPos, INT yPos)
{
	if (timer > 0 && player_turn == 0)
	{

		//무덤 클릭 시 카드 하나 뽑기
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
			//카드 내기가 가능할 경우
			//1. 카드 지우고
			//2. 턴 바꾸고
			//3. 타이머 다시 그리면서
			//4. 화면 리로드
			if (IsAllowToUse(selectCard))
			{
//				std::cout << "click : " << index << '\n';

				//카드 지우기
				GetNextCard()->owner = CARD_GRAVE;
				selectCard->owner = CARD_DECK;

				//턴 변경하기
				SetNextTurn(hWnd);
			}
		}
	}
}

VOID CardGameClass::TurnCPU(HWND hWnd)
{
	if (timer > 0)
	{
		std::vector<Card*> cpu_cards = GetPlayerCards(player_turn);
		Card* select_card;
		for (int i = 0; i < cpu_cards.size(); i++)
		{
			select_card = cpu_cards[i];
			if (IsAllowToUse(select_card))
			{
				//카드 지우기
				GetNextCard()->owner = CARD_GRAVE;
				select_card->owner = CARD_DECK;

				//턴 변경하기
				SetNextTurn(hWnd);
				return;
			}
		}

		GetCardFromGraves(player_turn);
		SetNextTurn(hWnd);
	}
}