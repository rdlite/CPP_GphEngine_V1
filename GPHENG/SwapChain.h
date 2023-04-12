#pragma once
#include "GraphicsEngine.h"
#include "d3d11.h"

class DeviceContext;

class SwapChain
{
public:
	bool init(HWND hwnd, UINT width, UINT height);
	bool present(bool vsync);
	bool release();
private:
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_rtv;

	friend DeviceContext;
};