#include "stdafx.h"
#include "GraphicClass.h"

GraphicClass::GraphicClass() :

	m_pDirect2DFactory(nullptr),
	m_pRenderTarget(nullptr),
	m_pLightSlateGrayBrush(nullptr),
	m_pCornflowerBlueBrush(nullptr)
{}

GraphicClass::~GraphicClass()
{
	SafeRelease(m_pDirect2DFactory);
	SafeRelease(m_pRenderTarget);
	SafeRelease(m_pLightSlateGrayBrush);
	SafeRelease(m_pCornflowerBlueBrush);
}


HRESULT GraphicClass::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	//싱글 쓰레드 팩토리 생성
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2DFactory);

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

HRESULT GraphicClass::OnRender(HWND hWnd)
{
	HRESULT hr = CreateDeviceResources(hWnd);

	if (SUCCEEDED(hr))
	{
		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
		D2D1_POINT_2F lt = D2D1::Point2F(0.f, 0.f);

		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		ID2D1Bitmap* bitmap;

		hr = LoadBitmapFromFile(L"image\\main_back.jpg", &bitmap);

		if (SUCCEEDED(hr))
		{
			m_pRenderTarget->DrawBitmap(
				bitmap,
				D2D1::RectF(lt.x, lt.y, lt.x + rtSize.width, lt.y + rtSize.height),
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