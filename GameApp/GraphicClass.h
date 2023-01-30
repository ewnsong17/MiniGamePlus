#pragma once

class GraphicClass
{
public:
	GraphicClass();
	~GraphicClass();

	HRESULT CreateDeviceIndependentResources(LPCWSTR fontName, float fontSize);
	HRESULT CreateDeviceResources(HWND hWnd);
	HRESULT OnRenderImage(HWND hWnd, D2D1_RECT_F rtSize, BOOL bReset = FALSE);
	HRESULT OnRenderText(HWND hWnd, LPCWSTR text, D2D1_SIZE_F rtSize, BOOL bReset = FALSE);
	VOID OnResize(UINT width, UINT height);
	HRESULT LoadBitmapFromFile(LPCWSTR uri, ID2D1Bitmap** ppBitmap);

	template <typename T>
	inline VOID SafeRelease(T*& p)
	{
		if (p != nullptr)
		{
			p->Release();
			p = nullptr;
		}
	}

	inline VOID DiscardDeviceResources()
	{
		SafeRelease(m_pRenderTarget);
		SafeRelease(m_pLightSlateGrayBrush);
		SafeRelease(m_pCornflowerBlueBrush);
	}

private:
	ID2D1Factory* m_pDirect2DFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;

	IWICImagingFactory* m_pIWICFactory;
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
};