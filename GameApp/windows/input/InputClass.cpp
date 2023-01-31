#include "stdafx.h"
#include "inputclass.h"
#include <card/CardGameClass.h>

void InputClass::Initialize()
{
	// 키 배열을 초기화합니다
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}
}


void InputClass::KeyDown(unsigned int input)
{
	// 키가 눌렸다면 해당 키값을 true로 저장합니다
	m_keys[input] = true;
}


void InputClass::KeyUp(unsigned int input)
{
	// 키가 해제되었다면 해당 키값을 false로 저장합니다
	m_keys[input] = false;
}


bool InputClass::IsKeyDown(unsigned int key)
{
	// 현재 키값이 눌려졌는지 아닌지 상태를 반환합니다
	return m_keys[key];
}

void InputClass::MouseDown(unsigned int input, unsigned int pos)
{
}

void InputClass::MouseUp(HWND hWnd, unsigned int input, unsigned int pos, IGame* game)
{
	int xPos = LOWORD(pos);
	int yPos = HIWORD(pos);

	if (CardGameClass* cardGame = dynamic_cast<CardGameClass*>(game))
	{
		if (game->player_turn == 0)
		{
			std::vector<Card*> myCards = cardGame->GetPlayerCards(0);

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
				if (cardGame->IsAllowToUse(selectCard))
				{
					std::cout << "click : " << index << '\n';

					//카드 지우기
					cardGame->GetNextCard()->owner = CARD_GRAVE;
					selectCard->owner = CARD_DECK;


					//턴 바꾸기
					game->player_turn++;

					//타이머 재설정
					game->timer = 30;

					InvalidateRect(hWnd, nullptr, TRUE);
				}
			}
		}
	}
}