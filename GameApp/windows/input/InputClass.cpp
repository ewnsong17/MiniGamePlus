#include "stdafx.h"
#include "inputclass.h"
#include <card/CardGameClass.h>

void InputClass::Initialize()
{
	// Ű �迭�� �ʱ�ȭ�մϴ�
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}
}


void InputClass::KeyDown(unsigned int input)
{
	// Ű�� ���ȴٸ� �ش� Ű���� true�� �����մϴ�
	m_keys[input] = true;
}


void InputClass::KeyUp(unsigned int input)
{
	// Ű�� �����Ǿ��ٸ� �ش� Ű���� false�� �����մϴ�
	m_keys[input] = false;
}


bool InputClass::IsKeyDown(unsigned int key)
{
	// ���� Ű���� ���������� �ƴ��� ���¸� ��ȯ�մϴ�
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
				//ī�� ���Ⱑ ������ ���
				//1. ī�� �����
				//2. �� �ٲٰ�
				//3. Ÿ�̸� �ٽ� �׸��鼭
				//4. ȭ�� ���ε�
				if (cardGame->IsAllowToUse(selectCard))
				{
					std::cout << "click : " << index << '\n';

					//ī�� �����
					cardGame->GetNextCard()->owner = CARD_GRAVE;
					selectCard->owner = CARD_DECK;


					//�� �ٲٱ�
					game->player_turn++;

					//Ÿ�̸� �缳��
					game->timer = 30;

					InvalidateRect(hWnd, nullptr, TRUE);
				}
			}
		}
	}
}