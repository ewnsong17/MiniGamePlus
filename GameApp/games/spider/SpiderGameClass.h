#pragma once
#include <IGame.h>
#include <system/SystemClass.h>

class SpiderCard
{
public:
	UINT number;
	BOOL bShow = FALSE;

	SpiderCard(INT _number):
		number(_number)
	{}
};

class SpiderCardList
{
public:
	FLOAT x = 0.f, y = 0.f;

	std::vector<SpiderCard*> card_vec;

	SpiderCardList(FLOAT _x, FLOAT _y) :
		x(_x),
		y(_y)
	{}
};

class SpiderGameClass : public IGame
{
public:
	SpiderGameClass(UINT player_size);

	std::wstring GetCardImage(SpiderCard* card);

	VOID MoveCard(SpiderCardList* moveTo);

	VOID CompleteSet();

	VOID GetCardFromGraves(HWND hWnd);

	virtual VOID GetMouseClick(HWND hWnd, INT xPos, INT yPos);

	std::vector<SpiderCardList*> card_list;

	std::vector<SpiderCard*> card_grave_list;

	INT grave_cnt = 5;

	INT select_index = -1;
	INT select_index_2 = -1;
};