#include "stdafx.h"
#include "GraphicClass.h"

GraphicClass::GraphicClass() :

	m_pDirect2DFactory(nullptr),
	m_pRenderTarget(nullptr),
	m_pCornSlikBrush(nullptr),
	m_pBlackBrush(nullptr),
	m_pYellowBrush(nullptr),
	m_pCornflowerBlueBrush(nullptr),
	m_pIWICFactory(nullptr),
	m_pDWriteFactory(nullptr),
	m_pTextFormat(nullptr),
	m_pTextFormat_2(nullptr)
{}

GraphicClass::~GraphicClass()
{
	SafeRelease(m_pDirect2DFactory);
	SafeRelease(m_pRenderTarget);
	SafeRelease(m_pCornSlikBrush);
	SafeRelease(m_pBlackBrush);
	SafeRelease(m_pYellowBrush);
	SafeRelease(m_pCornflowerBlueBrush);
	SafeRelease(m_pIWICFactory);
	SafeRelease(m_pDWriteFactory);
	SafeRelease(m_pTextFormat);
	SafeRelease(m_pTextFormat_2);
}


HRESULT GraphicClass::CreateDeviceIndependentResources(LPCWSTR fontName, float fontSize)
{
	HRESULT hr = S_OK;

	//싱글 쓰레드 팩토리 생성
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2DFactory);

	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pDWriteFactory->CreateTextFormat(
			fontName,
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			fontSize,
			L"kr",
			&m_pTextFormat
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pDWriteFactory->CreateTextFormat(
			fontName,
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			42.f,
			L"kr",
			&m_pTextFormat_2
		);
	}


	if (SUCCEEDED(hr))
	{
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		m_pTextFormat_2->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pTextFormat_2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	return hr;
}

HRESULT GraphicClass::CreateDeviceResources(HWND hWnd)
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		//HWND Render Target 생성하여 이미지 그릴 준비
		hr = m_pDirect2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, size), &m_pRenderTarget);

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Cornsilk), &m_pCornSlikBrush);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x202020), &m_pBlackBrush);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::GreenYellow), &m_pYellowBrush);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue), &m_pCornflowerBlueBrush);
		}
	}

	return hr;
}

VOID GraphicClass::OnRender(HWND hWnd, UINT& m_stageCnt, IGame* game)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);

	//TODO::상황별 텍스트 처리
	switch (m_stageCnt)
	{
		case START_GAME:
			this->OnRenderImage(hWnd, L"image\\game_title.jpg", D2D1::RectF(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT), TRUE);
//			this->OnRenderText(hWnd, L"미니게임마스터!", D2D1::SizeF(SCREEN_WIDTH, 200.f), m_pCornflowerBlueBrush);
			break;
		case SELECT_GAME:
			this->OnRenderImage(hWnd, L"image\\game_title.jpg", D2D1::RectF(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT), TRUE);
			this->OnRenderText(hWnd, L"원하시는 게임을 선택해주세요.", D2D1::SizeF(SCREEN_WIDTH, 200.f), m_pBlackBrush);
			break;
	}

	//TODO::게임 별 이미지 처리
	if (game != nullptr)
	{
		switch (m_stageCnt)
		{
			case CARD_GAME:
			case CARD_GAME_END:
				OnCardGameInit(hWnd, m_stageCnt == CARD_GAME_END, (CardGameClass*) game);
				break;
			case YUT_GAME:
			case YUT_GAME_END:
//				OnYutGameInit(hWnd, m_stageCnt == YUT_GAME_END, (YutGameClass*) game);
				break;
			case OMOK_GAME:
			case OMOK_GAME_END:
				OnOmokGameInit(hWnd, m_stageCnt == OMOK_GAME_END, (OmokGameClass*) game);
				break;
			case SPIDER_GAME:
			case SPIDER_GAME_END:
				OnSpiderGameInit(hWnd, m_stageCnt == SPIDER_GAME_END, (SpiderGameClass*) game);
				break;
		}
	}

	EndPaint(hWnd, &ps);
}

VOID GraphicClass::OnSpiderGameInit(HWND hWnd, BOOL bGameEnd, SpiderGameClass* game)
{
	ID2D1Bitmap* bitmap;
	D2D1_POINT_2F ltSize;

	D2D1_RECT_F rect;

	HRESULT hr = CreateDeviceResources(hWnd);

	//그리기 모드 시작
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		//배경화면 이미지 로딩
		hr = LoadBitmapFromFile(L"image\\game_card_back.jpg", &bitmap);

		if (SUCCEEDED(hr))
		{
			//배경화면 그리기
			m_pRenderTarget->DrawBitmap(
				bitmap,
				D2D1::RectF(-195.f, -40.f, SCREEN_WIDTH, SCREEN_HEIGHT),
				1.f
			);
		}

		//위쪽 카드 리스트 그리기
		for (int i = 0; i < game->card_list.size(); i++)
		{

			ltSize = D2D1::Point2F(game->card_list[i]->x, game->card_list[i]->y);
			rect = D2D1::RectF(ltSize.x, ltSize.y, ltSize.x + (0.8f * CARD_WIDTH), ltSize.y + (0.8f * CARD_HEIGHT));


			if (SUCCEEDED(hr))
			{
				for (int j = 0; j < game->card_list[i]->card_vec.size(); j++)
				{
					SpiderCard* card = game->card_list[i]->card_vec[j];

					if (card->bShow)
					{
						hr = LoadBitmapFromFile(game->GetCardImage(game->card_list[i]->card_vec[j]).c_str(), &bitmap);
					}
					else
					{
						hr = LoadBitmapFromFile(L"image\\card\\back.png", &bitmap);
					}

					m_pRenderTarget->DrawBitmap(
						bitmap,
						rect,
						1.f
					);

					rect.top += 18.5f;
					rect.bottom += 18.5f;
				}
			}
		}

		//오픈 된 카드 전체에 테두리
		if (game->select_index >= 0 && game->select_index_2 >= 0)
		{
			ltSize = D2D1::Point2F(game->card_list[game->select_index]->x, game->card_list[game->select_index]->y);
			rect = D2D1::RectF(ltSize.x, ltSize.y + (18.5f * game->select_index_2), ltSize.x + (0.8f * CARD_WIDTH), ltSize.y + (18.5f * (game->card_list[game->select_index]->card_vec.size() - 1)) + (0.8f * CARD_HEIGHT));

			m_pRenderTarget->DrawRectangle(rect, m_pYellowBrush, 2.5f);
		}

		//무덤 그리기
		for (int i = 0; i < game->grave_cnt; i++)
		{
			ltSize = D2D1::Point2F(875.f, 550.f + (i * 10));
			rect = D2D1::RectF(ltSize.x, ltSize.y, ltSize.x + (0.8f * CARD_WIDTH), ltSize.y + (0.8f * CARD_HEIGHT));

			hr = LoadBitmapFromFile(L"image\\card\\back.png", &bitmap);

			if (SUCCEEDED(hr))
			{
				m_pRenderTarget->DrawBitmap(
					bitmap,
					rect,
					1.f
				);
			}
		}


		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
}

VOID GraphicClass::OnYutGameInit(HWND hWnd, BOOL bGameEnd, YutGameClass* game)
{
	ID2D1Bitmap* bitmap;
	D2D1_POINT_2F ltSize;

	D2D1_SIZE_F rtSize;
	std::wstring text;

	HRESULT hr = CreateDeviceResources(hWnd);

	//그리기 모드 시작
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		//배경화면 이미지 로딩
		hr = LoadBitmapFromFile(L"image\\game_yut_back.jpg", &bitmap);

		if (SUCCEEDED(hr))
		{
			//배경화면 그리기
			m_pRenderTarget->DrawBitmap(
				bitmap,
				D2D1::RectF(-195.f, -40.f, SCREEN_WIDTH, SCREEN_HEIGHT),
				1.f
			);
		}

		//원 그리기
		ltSize = D2D1::Point2F(902.5f, 475.f);
		D2D1_ELLIPSE ellipse = D2D1::Ellipse(ltSize, 75.f, 75.f);
		m_pRenderTarget->DrawEllipse(ellipse, m_pBlackBrush, 3.5f);

		//글씨 그리기
		rtSize = D2D1::SizeF(SCREEN_WIDTH * 2 - 241.5f, 200.f);
		text = std::to_wstring(game->timer);

		m_pRenderTarget->DrawTextW(text.c_str(), wcslen(text.c_str()), m_pTextFormat_2, D2D1::RectF(0, 0, rtSize.width, rtSize.height), m_pCornSlikBrush);

		rtSize = D2D1::SizeF(SCREEN_WIDTH * 2 - 241.5f, 400.f);
		text = game->system_message;

		m_pRenderTarget->DrawTextW(text.c_str(), wcslen(text.c_str()), m_pTextFormat, D2D1::RectF(0, 0, rtSize.width, rtSize.height), m_pCornSlikBrush);


		rtSize = D2D1::SizeF(SCREEN_WIDTH * 2 - 241.5f, 650.f);

		text = game->yut_type;
		m_pRenderTarget->DrawTextW(text.c_str(), wcslen(text.c_str()), m_pTextFormat, D2D1::RectF(0, 0, rtSize.width, rtSize.height), m_pCornSlikBrush);


		rtSize = D2D1::SizeF(SCREEN_WIDTH * 2 - 241.5f, 950.f);

		text = L"윷 던지기";
		m_pRenderTarget->DrawTextW(text.c_str(), wcslen(text.c_str()), m_pTextFormat, D2D1::RectF(0, 0, rtSize.width, rtSize.height), m_pCornSlikBrush);

		for (auto iter = game->yut_player_vec.begin(); iter != game->yut_player_vec.end(); iter++)
		{
			rtSize = D2D1::SizeF(SCREEN_WIDTH * 2 - 326.5f + (50.f * (*iter)->index), (50 * (*iter)->owner) + 1150.f);

			if ((*iter)->bClear)
			{
				text = (*iter)->owner ? L"◆" : L"★";
			}
			else if (!(*iter)->bEnter)
			{
				text = (*iter)->owner ? L"◇" : L"☆";
			}

			m_pRenderTarget->DrawTextW(text.c_str(), wcslen(text.c_str()), m_pTextFormat, D2D1::RectF(0, 0, rtSize.width, rtSize.height), m_pCornSlikBrush);
		}

		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
}

VOID GraphicClass::OnOmokGameInit(HWND hWnd, BOOL bGameEnd, OmokGameClass* game)
{
	HRESULT hr = CreateDeviceResources(hWnd);

	ID2D1Bitmap* bitmap;
	D2D1_POINT_2F ltSize;

	D2D1_SIZE_F rtSize;
	std::wstring text;

	//그리기 모드 시작
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		//배경화면 이미지 로딩
		hr = LoadBitmapFromFile(L"image\\game_omok_back.jpg", &bitmap);

		if (SUCCEEDED(hr))
		{
			//배경화면 그리기
			ltSize = D2D1::Point2F(350.f, 275.f);

			m_pRenderTarget->DrawBitmap(
				bitmap,
				D2D1::RectF(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT),
				1.f
			);
		}

		float len = 750.f;
		float x = 15.f;
		while (x <= len)
		{
			m_pRenderTarget->DrawLine(D2D1::Point2F(x, 0.f), D2D1::Point2F(x, len), m_pCornSlikBrush, 1.5f);
			x += 35;
		}

		float y = 15.f;
		while (y < len)
		{
			m_pRenderTarget->DrawLine(D2D1::Point2F(0.f, y), D2D1::Point2F(len, y), m_pCornSlikBrush, 1.5f);
			y += 35;
		}

		for (auto iter = game->pos_list.begin(); iter != game->pos_list.end(); iter++)
		{
			if ((*iter)->bClick != CLICK_NONE)
			{
				text = L"●";

				m_pRenderTarget->DrawTextW(text.c_str(), wcslen(text.c_str()), m_pTextFormat_2, D2D1::RectF((*iter)->x, (*iter)->y, (*iter)->x, (*iter)->y),
					(*iter)->bClick == CLICK_WHITE ? m_pCornSlikBrush : m_pBlackBrush);
			}
		}

		rtSize = D2D1::SizeF(SCREEN_WIDTH * 2 - 300.f, 130.f);

		//텍스트 그리기
		if (bGameEnd)
		{
			text = std::wstring(game->player_vector[game->player_turn].player_name) + L"의 승리입니다!";
			m_pRenderTarget->DrawTextW(text.c_str(), wcslen(text.c_str()), m_pTextFormat, D2D1::RectF(0, 0, rtSize.width, rtSize.height), m_pCornSlikBrush);
		}
		else
		{

			text = std::to_wstring(game->timer);

			m_pRenderTarget->DrawTextW(text.c_str(), wcslen(text.c_str()), m_pTextFormat_2, D2D1::RectF(0, 0, rtSize.width, rtSize.height), m_pCornSlikBrush);

			rtSize = D2D1::SizeF(SCREEN_WIDTH * 2 - 285.f, 250.f);

			if (game->player_turn)
			{
				text = L"적의 차례입니다.";
				m_pRenderTarget->DrawTextW(text.c_str(), wcslen(text.c_str()), m_pTextFormat, D2D1::RectF(0, 0, rtSize.width, rtSize.height), m_pCornSlikBrush);
			}
			else
			{
				text = L"나의 차례입니다.";
				m_pRenderTarget->DrawTextW(text.c_str(), wcslen(text.c_str()), m_pTextFormat, D2D1::RectF(0, 0, rtSize.width, rtSize.height), m_pCornSlikBrush);
			}
		}

		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
}

VOID GraphicClass::OnCardGameInit(HWND hWnd, BOOL bGameEnd, CardGameClass* game)
{

	D2D1_POINT_2F ltSize;
	D2D1_POINT_2F imageCenter;

	ID2D1Bitmap* bitmap;

	D2D1_SIZE_F rtSize;
	std::wstring text;

	std::vector<Card*> card_list;

	HRESULT hr = CreateDeviceResources(hWnd);

	//그리기 모드 시작
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		//배경화면 이미지 로딩
		hr = LoadBitmapFromFile(L"image\\game_card_back.jpg", &bitmap);

		if (SUCCEEDED(hr))
		{
			//배경화면 그리기
			ltSize = D2D1::Point2F(350.f, 275.f);

			m_pRenderTarget->DrawBitmap(
				bitmap,
				D2D1::RectF(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT),
				1.f
			);
		}

		//내 카드리스트 그려주기
		card_list = game->GetPlayerCards(0);

		for (int i = 0; i < card_list.size(); i++)
		{
			//카드 이미지 각각 로딩
			hr = LoadBitmapFromFile(game->GetCardImage(card_list[i]).c_str(), &bitmap);

			if (SUCCEEDED(hr))
			{
				ltSize = D2D1::Point2F(350.f + (i * 30), 620.f);

				D2D1_RECT_F rect = D2D1::RectF(ltSize.x, ltSize.y, ltSize.x + CARD_WIDTH, ltSize.y + CARD_HEIGHT);

				card_list[i]->rect = rect;

				m_pRenderTarget->DrawBitmap(
					bitmap,
					rect,
					1.f
				);
			}
		}

		Card* next_card = game->GetNextCard();

		//다음 카드 로딩
		hr = LoadBitmapFromFile(game->GetCardImage(next_card).c_str(), &bitmap);

		if (SUCCEEDED(hr))
		{
			//다음 카드 그려주기

			ltSize = D2D1::Point2F(450.f, 250.f);

			m_pRenderTarget->DrawBitmap(
				bitmap,
				D2D1::RectF(ltSize.x, ltSize.y, ltSize.x + CARD_WIDTH, ltSize.y + CARD_HEIGHT),
				1.f
			);
		}

		//뒷면 카드 로딩
		hr = LoadBitmapFromFile(L"image\\Card\\back.png", &bitmap);

		if (SUCCEEDED(hr))
		{
			//무덤 그려주기
			ltSize = D2D1::Point2F(350.f, 275.f);
			D2D1_RECT_F rect = D2D1::RectF(ltSize.x, ltSize.y, ltSize.x + (0.66f * CARD_WIDTH), ltSize.y + (0.66f * CARD_HEIGHT));

			game->grave_rect = rect;

			m_pRenderTarget->DrawBitmap(
				bitmap,
				rect,
				1.f
			);

			//왼쪽 플레이어
			card_list = game->GetPlayerCards(1);

			ltSize = D2D1::Point2F(150.f, 210.f);
			imageCenter = D2D1::Point2F(
				ltSize.x + CARD_WIDTH / 2,
				ltSize.y + CARD_HEIGHT / 2
			);

			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(-90, imageCenter));

			for (int i = 0; i < card_list.size(); i++)
			{
				ltSize.x -= 30;

				m_pRenderTarget->DrawBitmap(
					bitmap,
					D2D1::RectF(ltSize.x, ltSize.y, ltSize.x + CARD_WIDTH, ltSize.y - CARD_HEIGHT),
					1.f
				);
			}

			//위쪽 플레이어
			card_list = game->GetPlayerCards(2);

			ltSize = D2D1::Point2F(450.f, 110.f);
			imageCenter = D2D1::Point2F(
				ltSize.x + CARD_WIDTH / 2,
				ltSize.y + CARD_HEIGHT / 2
			);

			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(-360, imageCenter));

			for (int i = 0; i < card_list.size(); i++)
			{
				ltSize.x -= 30;

				m_pRenderTarget->DrawBitmap(
					bitmap,
					D2D1::RectF(ltSize.x, ltSize.y, ltSize.x + CARD_WIDTH, ltSize.y - CARD_HEIGHT),
					1.f
				);
			}

			//왼쪽 플레이어
			card_list = game->GetPlayerCards(3);

			ltSize = D2D1::Point2F(1040.f, 150.f);
			imageCenter = D2D1::Point2F(
				ltSize.x + CARD_WIDTH / 2,
				ltSize.y + CARD_HEIGHT / 2
			);

			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(-90, imageCenter));

			for (int i = 0; i < card_list.size(); i++)
			{
				ltSize.x -= 30;

				m_pRenderTarget->DrawBitmap(
					bitmap,
					D2D1::RectF(ltSize.x, ltSize.y, ltSize.x + CARD_WIDTH, ltSize.y - CARD_HEIGHT),
					1.f
				);
			}
		}

		//글씨 그리기 전 각도 초기화
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		//글씨들 전부 그리기
		rtSize = D2D1::SizeF(SCREEN_WIDTH, 300.f);

		text = game->special_text;

		m_pRenderTarget->DrawTextW(text.c_str(), wcslen(text.c_str()), m_pTextFormat, D2D1::RectF(0, 0, rtSize.width, rtSize.height), m_pCornSlikBrush);

		rtSize = D2D1::SizeF(SCREEN_WIDTH, 360.f);

		if (bGameEnd)
		{
			text = std::wstring(game->player_vector.at(game->player_turn).player_name) + L"의 승리입니다!";
		}
		else
		{
			text = std::wstring(game->player_vector.at(game->player_turn).player_name) + L"의 차례입니다.";
		}

		m_pRenderTarget->DrawTextW(text.c_str(), wcslen(text.c_str()), m_pTextFormat, D2D1::RectF(0, 0, rtSize.width, rtSize.height), m_pCornSlikBrush);


		if (!bGameEnd)
		{
			rtSize = D2D1::SizeF(SCREEN_WIDTH - 20.f, 430.f);
			text = std::to_wstring(game->timer);

			m_pRenderTarget->DrawTextW(text.c_str(), wcslen(text.c_str()), m_pTextFormat, D2D1::RectF(0, 0, rtSize.width, rtSize.height), m_pCornSlikBrush);
		}

		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
}

HRESULT GraphicClass::OnRenderImage(HWND hWnd, LPCWSTR uri, D2D1_RECT_F rtSize, BOOL bReset)
{
	HRESULT hr = CreateDeviceResources(hWnd);

	if (SUCCEEDED(hr))
	{

		m_pRenderTarget->BeginDraw();

		if (bReset)
		{
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		}

		ID2D1Bitmap* bitmap;

		hr = LoadBitmapFromFile(uri, &bitmap);

		if (SUCCEEDED(hr))
		{
			m_pRenderTarget->DrawBitmap(
				bitmap,
				rtSize,
				1.f
			);
		}

		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}

HRESULT GraphicClass::OnRenderText(HWND hWnd, LPCWSTR text, D2D1_SIZE_F rtSize, ID2D1SolidColorBrush* brush, BOOL bReset)
{
	HRESULT hr = CreateDeviceResources(hWnd);

	if (SUCCEEDED(hr))
	{

		m_pRenderTarget->BeginDraw();

		if (bReset)
		{
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		}

		m_pRenderTarget->DrawTextW(text, wcslen(text), m_pTextFormat, D2D1::RectF(0, 0, rtSize.width, rtSize.height), brush);

		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}

VOID GraphicClass::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

HRESULT GraphicClass::LoadBitmapFromFile(LPCWSTR uri, ID2D1Bitmap** ppBitmap)
{
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pSource = nullptr;
	IWICFormatConverter* pConverter = nullptr;

	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&m_pIWICFactory));

	if (SUCCEEDED(hr))
	{
		hr = m_pIWICFactory->CreateDecoderFromFilename(uri, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
	}

	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pIWICFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateBitmapFromWicBitmap(pConverter, nullptr, ppBitmap);
	}

	SafeRelease(pDecoder);
	SafeRelease(pSource);
	SafeRelease(pConverter);

	return hr;
}