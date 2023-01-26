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

	VOID CreateBitMap(LPCWSTR imagePath);

private:
	ID2D1Factory* g_ipD2DFactory = nullptr;
	IWICImagingFactory* g_ipWICFactory = nullptr;
	IWICFormatConverter* g_ipConvertedSrcBmp = nullptr;
	ID2D1Bitmap* g_ipD2DBitmap = nullptr;
	ID2D1HwndRenderTarget* g_ipRT = nullptr;
};