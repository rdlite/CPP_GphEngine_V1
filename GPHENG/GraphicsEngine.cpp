#include "GraphicsEngine.h"

bool GraphicsEngine::init()
{
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};

	UINT driverTypesAmount = ARRAYSIZE(driverTypes);
	UINT featureLevelsAmount = ARRAYSIZE(featureLevels);

	HRESULT result = 0;

	for (UINT i = 0; i < driverTypesAmount; i++)
	{
		result = D3D11CreateDevice(
			NULL,
			driverTypes[i],
			NULL,
			NULL, 
			featureLevels,
			featureLevelsAmount,
			D3D11_SDK_VERSION,
			&m_d3dDevice,
			&m_featureLevel,
			&m_immContext);

		if (SUCCEEDED(result)) 
		{
			break;
		}
	}

	if (FAILED(result))
	{
		return false;
	}

	m_deviceContext = new DeviceContext(m_immContext);

	m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgiDevice);
	m_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgiAdapter);
	m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory);

	return true;
}

bool GraphicsEngine::release()
{
	if (m_vs)m_vs->Release();
	if (m_ps)m_ps->Release();

	if (m_vsblob)m_vsblob->Release();
	if (m_psblob)m_psblob->Release();

	m_dxgiDevice->Release();
	m_dxgiAdapter->Release();
	m_dxgiFactory->Release();

	m_deviceContext->release();
	m_d3dDevice->Release();

	return true;
}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}

DeviceContext* GraphicsEngine::getImmediateDeviceContext()
{
	return this->m_deviceContext;
}

VertexBuffer* GraphicsEngine::createVertexBuffer()
{
	return new VertexBuffer();
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}

void GraphicsEngine::createShaders()
{
	ID3DBlob* errblob = nullptr;
	D3DCompileFromFile(L"shader.fx", nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &m_vsblob, &errblob);
	D3DCompileFromFile(L"shader.fx", nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &m_psblob, &errblob);
	m_d3dDevice->CreateVertexShader(m_vsblob->GetBufferPointer(), m_vsblob->GetBufferSize(), nullptr, &m_vs);
	m_d3dDevice->CreatePixelShader(m_psblob->GetBufferPointer(), m_psblob->GetBufferSize(), nullptr, &m_ps);
}

void GraphicsEngine::setShaders()
{
	m_immContext->VSSetShader(m_vs, nullptr, 0);
	m_immContext->PSSetShader(m_ps, nullptr, 0);
}

void GraphicsEngine::getShaderBufferAndSize(void** bytecode, UINT* size)
{
	*bytecode = this->m_vsblob->GetBufferPointer();
	*size = (UINT)this->m_vsblob->GetBufferSize();
}