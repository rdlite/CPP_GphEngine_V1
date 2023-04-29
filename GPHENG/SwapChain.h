#pragma once
#include "GraphicsEngine.h"
#include "Prerequisites.h"
#include <exception>

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);
	bool present(bool vsync);
	~SwapChain();
private:
	RenderSystem* m_system = nullptr;

	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_rtv;

	friend DeviceContext;
};