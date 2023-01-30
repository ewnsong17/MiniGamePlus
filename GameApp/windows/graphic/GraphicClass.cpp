#include "stdafx.h"
#include "GraphicClass.h"

GraphicClass::GraphicClass() :

	m_pDirect2DFactory(nullptr),
	m_pRenderTarget(nullptr),
	m_pLightSlateGrayBrush(nullptr),
	m_pCornflowerBlueBrush(nullptr),
	m_pIWICFactory(nullptr),
	m_pDWriteFactory(nullptr),
	m_pTextFormat(nullptr)
{}

GraphicClass::~GraphicClass()
{
	SafeRelease(m_pDirect2DFactory);
	SafeRelease(m_pRenderTarget);
	SafeRelease(m_pLightSlateGrayBrush);
	SafeRelease(m_pCornflowerBlueBrush);
	SafeRelease(m_pIWICFactory);
	SafeRelease(m_pDWriteFactory);
	SafeRelease(m_pTextFormat);
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

		//HWND Render Target 생성하여 이미지 그릴 준비
		hr = m_pDirect2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, size), &m_pRenderTarget);

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightSlateGray), &m_pLightSlateGrayBrush);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue), &m_pCornflowerBlueBrush);
		}
	}

	return hr;
}

VOID GraphicClass::OnRender(HWND hWnd, UINT m_stageCnt)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);

	//TODO::상황별 이미지 처리
	if (m_stageCnt <= SELECT_GAME)
	{
		this->OnRenderImage(hWnd, L"image\\main_back.jpg", D2D1::RectF(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT), TRUE);
	}
	else
	{
		this->OnRenderImage(hWnd, L"image\\game_back.jpg", D2D1::RectF(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT), TRUE);
	}

	//TODO::상황별 텍스트 처리
	switch (m_stageCnt)
	{
		case START_GAME:
			this->OnRenderText(hWnd, L"미니게임마스터!", D2D1::SizeF(SCREEN_WIDTH, 200.f));
			break;
		case SELECT_GAME:
			this->OnRenderText(hWnd, L"원하시는 게임을 선택해주세요.", D2D1::SizeF(SCREEN_WIDTH, 200.f));
			break;
	}

	EndPaint(hWnd, &ps);
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

HRESULT GraphicClass::OnRenderText(HWND hWnd, LPCWSTR text, D2D1_SIZE_F rtSize, BOOL bReset)
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

		m_pRenderTarget->DrawTextW(text, wcslen(text), m_pTextFormat, D2D1::RectF(0, 0, rtSize.width, rtSize.height), m_pCornflowerBlueBrush);

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