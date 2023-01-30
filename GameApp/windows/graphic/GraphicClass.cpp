#include "stdafx.h"
#include "GraphicClass.h"

GraphicClass::GraphicClass() :

	m_pDirect2DFactory(nullptr),
	m_pRenderTarget(nullptr),
	m_pCornSlikBrush(nullptr),
	m_pCornflowerBlueBrush(nullptr),
	m_pIWICFactory(nullptr),
	m_pDWriteFactory(nullptr),
	m_pTextFormat(nullptr)
{}

GraphicClass::~GraphicClass()
{
	SafeRelease(m_pDirect2DFactory);
	SafeRelease(m_pRenderTarget);
	SafeRelease(m_pCornSlikBrush);
	SafeRelease(m_pCornflowerBlueBrush);
	SafeRelease(m_pIWICFactory);
	SafeRelease(m_pDWriteFactory);
	SafeRelease(m_pTextFormat);
}


HRESULT GraphicClass::CreateDeviceIndependentResources(LPCWSTR fontName, float fontSize)
{
	HRESULT hr = S_OK;

	//�̱� ������ ���丮 ����
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
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
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

		//HWND Render Target �����Ͽ� �̹��� �׸� �غ�
		hr = m_pDirect2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, size), &m_pRenderTarget);

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Cornsilk), &m_pCornSlikBrush);
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

	//TODO::��Ȳ�� �ؽ�Ʈ ó��
	switch (m_stageCnt)
	{
		case START_GAME:
			this->OnRenderImage(hWnd, L"image\\game_back.jpg", D2D1::RectF(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT), TRUE);
			this->OnRenderText(hWnd, L"�̴ϰ��Ӹ�����!", D2D1::SizeF(SCREEN_WIDTH, 200.f), m_pCornflowerBlueBrush);
			break;
		case SELECT_GAME:
			this->OnRenderImage(hWnd, L"image\\game_back.jpg", D2D1::RectF(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT), TRUE);
			this->OnRenderText(hWnd, L"���Ͻô� ������ �������ּ���.", D2D1::SizeF(SCREEN_WIDTH, 200.f), m_pCornflowerBlueBrush);
			break;
	}

	//TODO::���� �� �̹��� ó��
	if (game != nullptr)
	{
		switch (m_stageCnt)
		{
			case CARD_GAME:
				this->OnRenderImage(hWnd, L"image\\main_back.jpg", D2D1::RectF(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT), TRUE);
				OnCardGameInit(hWnd, (CardGameClass*) game);
				break;
			case YUT_GAME:
				this->OnRenderImage(hWnd, L"image\\main_back.jpg", D2D1::RectF(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT), TRUE);
				break;
			case OMOK_GAME:
				this->OnRenderImage(hWnd, L"image\\main_back.jpg", D2D1::RectF(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT), TRUE);
				break;
		}
	}

	EndPaint(hWnd, &ps);
}

VOID GraphicClass::OnCardGameInit(HWND hWnd, CardGameClass* game)
{
	//�� ���
	if (game->player_turn == 0)
	{
		this->OnRenderText(hWnd, L"����� �����Դϴ�.", D2D1::SizeF(SCREEN_WIDTH, 350.f), m_pCornSlikBrush);
	}
	else
	{
		std::wstring str = std::to_wstring(game->player_turn) + L"���� �����Դϴ�.";
		this->OnRenderText(hWnd, str.c_str(), D2D1::SizeF(SCREEN_WIDTH, 350.f), m_pCornSlikBrush);
	}


	this->OnRenderText(hWnd, std::to_wstring(game->timer).c_str(), D2D1::SizeF(SCREEN_WIDTH - 20.f, 430.f), m_pCornSlikBrush);

	D2D1_POINT_2F ltSize;
	D2D1_POINT_2F imageCenter;

	//���� �׷��ֱ�
	ltSize = D2D1::Point2F(350.f, 275.f);

	this->OnRenderImage(hWnd, L"image\\Card\\back.png", D2D1::RectF(ltSize.x, ltSize.y, ltSize.x + (0.66f * CARD_WIDTH), ltSize.y + (0.66f * CARD_HEIGHT)), FALSE);



	//���� ī�� �׷��ֱ�
	Card* next_card = game->GetNextCard();
	
	ltSize = D2D1::Point2F(450.f, 250.f);

	this->OnRenderImage(hWnd, game->GetCardImage(next_card).c_str(), D2D1::RectF(ltSize.x, ltSize.y, ltSize.x + CARD_WIDTH, ltSize.y + CARD_HEIGHT), FALSE);


	//ī�� �׸� �׷��ֱ�
	std::vector<Card*> card_list;

	//TODO::���� �迭�� ��ȯ, ���� ��ȯ�� ������� �켱 �ϵ��ڵ� ó��

	//���� �׷��ֱ�
	card_list = game->GetPlayerCards(0);

	for (int i = 0; i < card_list.size(); i++)
	{
		ltSize = D2D1::Point2F(350.f + (i * 30), 620.f);

		this->OnRenderImage(hWnd, game->GetCardImage(card_list[i]).c_str(), D2D1::RectF(ltSize.x, ltSize.y, ltSize.x + CARD_WIDTH, ltSize.y + CARD_HEIGHT), FALSE);
	}



	//���� �÷��̾�
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

		this->OnRenderImage(hWnd, L"image\\Card\\back.png", D2D1::RectF(ltSize.x, ltSize.y, ltSize.x + CARD_WIDTH, ltSize.y - CARD_HEIGHT), FALSE);
	}



	//���� �÷��̾�
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
		this->OnRenderImage(hWnd, L"image\\Card\\back.png", D2D1::RectF(ltSize.x, ltSize.y, ltSize.x + CARD_WIDTH, ltSize.y - CARD_HEIGHT), FALSE);
	}



	//���� �÷��̾�
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
		this->OnRenderImage(hWnd, L"image\\Card\\back.png", D2D1::RectF(ltSize.x, ltSize.y, ltSize.x + CARD_WIDTH, ltSize.y - CARD_HEIGHT), FALSE);
	}


	//���� �ʱ�ȭ
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
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