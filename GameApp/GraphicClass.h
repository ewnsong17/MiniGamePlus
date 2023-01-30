#pragma once

class GraphicClass
{
public:
	GraphicClass();
	~GraphicClass();

	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources(HWND hWnd);
	HRESULT OnRender(HWND hWnd);
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
};