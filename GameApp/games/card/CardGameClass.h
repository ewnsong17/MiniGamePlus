#pragma once
#include "IGame.h"

constexpr INT CARD_GRAVE = -2;
constexpr INT CARD_DECK = -1;

constexpr UINT CLOVER = 0;
constexpr UINT DIAMOND = 1;
constexpr UINT HEART = 2;
constexpr UINT SPADE = 3;

constexpr UINT CARD_WIDTH = 110;
constexpr UINT CARD_HEIGHT = 154;

class Card
{
public:
	UINT color;
	UINT number;
	INT owner = CARD_DECK;
	D2D1_RECT_F rect{};

	Card(UINT _color, UINT _number) :
		color(_color),
		number(_number)
	{}
};

class CardGameClass : public IGame
{
public:
	std::vector<Card*> card_list;

	CardGameClass(UINT player_size);

	Card* GetNextCard();

	std::vector<Card*> GetPlayerCards(int index);

	std::wstring GetCardImage(Card* card);

	BOOL IsAllowToUse(Card* card);
};