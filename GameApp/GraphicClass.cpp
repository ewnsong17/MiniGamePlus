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

	HRESULT hr = E_FAIL;

	if (g_ipRT != nullptr)
	{
		RECT r;
		GetClientRect(hWnd, &r);

		//hDC ����ȭ
		g_ipRT->BindDC(hDC, &r);


		//���� ȭ�� �ʱ�ȭ
		g_ipRT->BeginDraw();
		g_ipRT->SetTransform(D2D1::Matrix3x2F::Identity());
		g_ipRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

		//�ε��� �̹����� ���� ��� �׸���
		if (!FAILED(LoadBitMap(L"image\\main_back.jpg")))
		{
			D2D1_RECT_F dxArea = D2D1::RectF(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
			g_ipRT->DrawBitmap(g_ipD2DBitmap, dxArea);
		}

		//�۾� ��Ʈ �ε�
		if (!FAILED(LoadFont(L"����", 40.f)))
		{
			LPCWSTR str = L"�̴ϰ��Ӹ�����!";
			g_ipRT->DrawTextW(str, wcslen(str), g_writeFormat, D2D1::RectF(0, 0, SCREEN_WIDTH, 200.f), g_brush);
		}

		//�׸��� ����
		hr = g_ipRT->EndDraw();
	}

	EndPaint(hWnd, &ps);
}

VOID GraphicClass::Initialize(HWND hWnd)
{
	HRESULT hr = E_FAIL;

	RECT r;
	GetClientRect(hWnd, &r);

	//Windows Imaging Component Factory ����
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&g_ipWICFactory));

	assert(hr == S_OK);

	//DCRenderTarget ���� (������ â�� ������)
	auto props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_IGNORE),
		0,
		0,
		D2D1_RENDER_TARGET_USAGE_NONE,
		D2D1_FEATURE_LEVEL_DEFAULT
	);

	//D2D1 Factory ����
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_ipD2DFactory);

	hr = g_ipD2DFactory->CreateDCRenderTarget(
		&props,
		&g_ipRT
	);

	assert(hr == S_OK);
}

HRESULT GraphicClass::LoadBitMap(LPCWSTR imagePath)
{
	IWICBitmapDecoder* ipDecoderPtr = nullptr;
	
	HRESULT hr = E_FAIL;

	//���ڴ� ����
	hr = g_ipWICFactory->CreateDecoderFromFilename(imagePath, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &ipDecoderPtr);

	assert(hr == S_OK);

	//���ڴ����� ������ ���
	IWICBitmapFrameDecode* ipFramePtr = nullptr;
	hr = ipDecoderPtr->GetFrame(0, &ipFramePtr);
	assert(hr == S_OK);

	//�������� ������� ���� ������ ����
	SafeRelease(g_ipConvertedSrcBmp);
	hr = g_ipWICFactory->CreateFormatConverter(&g_ipConvertedSrcBmp);
	assert(hr == S_OK);

	hr = g_ipConvertedSrcBmp->Initialize(ipFramePtr, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeMedianCut);

	assert(hr == S_OK);

	//�����ͷ� ���� ��Ʈ�� ����
	SafeRelease(g_ipD2DBitmap);
	hr = g_ipRT->CreateBitmapFromWicBitmap(g_ipConvertedSrcBmp, nullptr, &g_ipD2DBitmap);

	assert(hr == S_OK);

	SafeRelease(ipDecoderPtr);
	SafeRelease(ipFramePtr);

	return hr;
}

HRESULT GraphicClass::LoadFont(LPCWSTR fontName, FLOAT fontSize)
{
	HRESULT hr = E_FAIL;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(g_writeFactory), (IUnknown**)&g_writeFactory);

	assert(hr == S_OK);

	hr = g_writeFactory->CreateTextFormat(
		fontName,
		0,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"ko",
		&g_writeFormat);

	g_writeFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	g_writeFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	assert(hr == S_OK);

	hr = g_ipRT->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f), &g_brush);

	return hr;
}
