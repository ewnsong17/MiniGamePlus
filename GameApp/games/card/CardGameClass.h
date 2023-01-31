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
	std::wstring special_text = L"";
	D2D1_RECT_F grave_rect{};
	BOOL BLOCK_TURN_SET = FALSE;
	BOOL TURN_REVERSE = FALSE;
	BOOL TURN_JUMP = FALSE;
	BOOL SELECT_COLOR = FALSE;
	UINT ATTACK_CNT = 0;
	UINT DEAD_LINE = 15;
	Card* FAKE_CARD = nullptr;

	CardGameClass(UINT player_size);

	Card* GetNextCard();

	VOID GetCardFromGraves(int ownerID);

	VOID SetNextTurn(HWND hWnd);

	std::vector<Card*> GetPlayerCards(int index);

	std::wstring GetCardImage(Card* card);

	BOOL IsAllowToUse(Card* card);

	VOID GetMouseClick(HWND hWnd, INT xPos, INT yPos);

	VOID TurnCPU(HWND hWnd);

	VOID ShowCardEffect(Card* selectCard);

	VOID PlayerDead(int playerID);

	VOID SetGameEnd();

	VOID SetColor(HWND hWnd, DWORD color);
};