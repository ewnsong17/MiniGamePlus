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
	INT owner = CARD_GRAVE;
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
	D2D1_RECT_F grave_rect{};

	CardGameClass(UINT player_size);

	Card* GetNextCard();

	VOID GetCardFromGraves(int ownerID);

	VOID SetNextTurn(HWND hWnd);

	std::vector<Card*> GetPlayerCards(int index);

	std::wstring GetCardImage(Card* card);

	BOOL IsAllowToUse(Card* card);

	VOID GetMouseClick(HWND hWnd, INT xPos, INT yPos);

	VOID TurnCPU(HWND hWnd);

	virtual VOID SetGameEnd(HWND hWnd);
};