#include "SwapChain.h"
#include "iostream"

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system) : m_system(system)
{
	ID3D11Device* device = m_system->m_d3dDevice;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Windowed = TRUE;

	HRESULT result = m_system->m_dxgiFactory->CreateSwapChain(device, &desc, &m_swapChain);

	if (FAILED(result))
	{
		throw std::exception("SwapChain not created successfully");
	}

	reloadBuffers(width, height);
}

void SwapChain::resize(unsigned int width, unsigned int height)
{
	if (m_rtv) m_rtv->Release();
	if (m_dsv) m_dsv->Release();

	m_swapChain->ResizeBuffers(
		1, width, height, 
		DXGI_FORMAT_R8G8B8A8_UNORM, 
		0);

	reloadBuffers(width, height);
}

void SwapChain::setFullScreen(bool isFull, unsigned int width, unsigned int height)
{
	resize(width, height);
	m_swapChain->SetFullscreenState(isFull, nullptr);
}

bool SwapChain::present(bool vsync)
{
	m_swapChain->Present(vsync, NULL);

	return true;
}

void SwapChain::reloadBuffers(unsigned int width, unsigned int height)
{
	ID3D11Texture2D* buffer = NULL;
	HRESULT result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(result))
	{
		throw std::exception("SwapChain not created successfully");
	}

	result = m_system->m_d3dDevice->CreateRenderTargetView(buffer, NULL, &m_rtv);
	buffer->Release();

	if (FAILED(result))
	{
		throw std::exception("SwapChain not created successfully");
	}

	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = width;
	tex_desc.Height = height;
	tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;

	result = m_system->m_d3dDevice->CreateTexture2D(&tex_desc, nullptr, &buffer);

	if (FAILED(result))
	{
		throw std::exception("SwapChain not created successfully");
	}

	result = m_system->m_d3dDevice->CreateDepthStencilView(buffer, NULL, &m_dsv);
	buffer->Release();

	if (FAILED(result))
	{
		throw std::exception("SwapChain not created successfully");
	}
}

SwapChain::~SwapChain()
{
	m_swapChain->Release();
}