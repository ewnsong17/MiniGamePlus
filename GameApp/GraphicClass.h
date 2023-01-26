#pragma once

class GraphicClass
{
public:
	VOID PaintScreen(HWND hWnd);

	VOID Initialize(HWND hWnd);

	template <typename T>
	inline VOID SafeRelease(T*& p)
	{
		if (p != nullptr)
		{
			p->Release();
			p = nullptr;
		}
	}

	HRESULT LoadBitMap(LPCWSTR imagePath);

	HRESULT LoadFont(LPCWSTR fontName, FLOAT fontSize);

	
private:
	ID2D1Factory* g_ipD2DFactory = nullptr;
	IWICImagingFactory* g_ipWICFactory = nullptr;
	IWICFormatConverter* g_ipConvertedSrcBmp = nullptr;
	ID2D1Bitmap* g_ipD2DBitmap = nullptr;
	ID2D1DCRenderTarget* g_ipRT = nullptr;

	IDWriteFactory* g_writeFactory = nullptr;
	IDWriteTextFormat* g_writeFormat = nullptr;
	IDWriteTextLayout* g_writeLayout = nullptr;

	ID2D1SolidColorBrush* g_brush = nullptr;
};