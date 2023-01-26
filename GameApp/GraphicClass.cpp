#include "stdafx.h"
#include "GraphicClass.h"

VOID GraphicClass::PaintScreen(HWND hWnd)
{
	if (this == nullptr)
	{
		return;
	}

	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps);

	if (g_ipRT == nullptr)
	{
	//	//		InitializeRT(hWnd);
	//		//	AutoLoadImage
	}
	else
	{
		HRESULT hr = E_FAIL;
		g_ipRT->BeginDraw();
		g_ipRT->SetTransform(D2D1::Matrix3x2F::Identity());
		g_ipRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

		if (g_ipD2DBitmap != nullptr)
		{
			D2D1_RECT_F dxArea = D2D1::RectF(0.0f, 0.0f, 300.0f, 300.0f);
			g_ipRT->DrawBitmap(g_ipD2DBitmap, dxArea);
		}

		hr = g_ipRT->EndDraw();
	}

	EndPaint(hWnd, &ps);
}

VOID GraphicClass::Initialize(HWND hWnd)
{
	HRESULT hr = E_FAIL;

	//싱글 스레드로 D2D Factory 생성
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_ipD2DFactory);

	assert(hr == S_OK);

	RECT r;
	GetClientRect(hWnd, &r);

	g_ipD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(r.right - r.left, r.bottom - r.top)),
		&g_ipRT
	);

	//Windows Imaging Component Factory 생성
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_ipWICFactory));

	assert(hr == S_OK);

	CreateBitMap(L"background.jpg");
}

VOID GraphicClass::CreateBitMap(LPCWSTR imagePath)
{
	IWICBitmapDecoder* ipDecoderPtr = nullptr;
	
	HRESULT hr = E_FAIL;

	//디코더 생성
	hr = g_ipWICFactory->CreateDecoderFromFilename(L"C:\\Users\\R0720H\\Desktop\\Etc\\Git\\MiniGamePlus\\GameApp\\x64\\Debug\\main_back.jpg", nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &ipDecoderPtr);

	assert(hr == S_OK);

	//디코더에서 프레임 얻기
	IWICBitmapFrameDecode* ipFramePtr = nullptr;
	hr = ipDecoderPtr->GetFrame(0, &ipFramePtr);
	assert(hr == S_OK);

	//프레임을 기반으로 포맷 컨버터 생성
	SafeRelease(g_ipConvertedSrcBmp);
	hr = g_ipWICFactory->CreateFormatConverter(&g_ipConvertedSrcBmp);
	assert(hr == S_OK);

	hr = g_ipConvertedSrcBmp->Initialize(ipFramePtr, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);

	assert(hr == S_OK);

	//컨버터로 실제 비트맵 생성
	SafeRelease(g_ipD2DBitmap);
	hr = g_ipRT->CreateBitmapFromWicBitmap(g_ipConvertedSrcBmp, nullptr, &g_ipD2DBitmap);

	assert(hr == S_OK);

	SafeRelease(ipDecoderPtr);
	SafeRelease(ipFramePtr);

}