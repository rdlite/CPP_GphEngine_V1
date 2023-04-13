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

VertexShader* GraphicsEngine::createVertexShader(const void* shaderByteCode, size_t byteCodeSize)
{
	VertexShader* vertexShader = new VertexShader();

	if (!vertexShader->init(shaderByteCode, byteCodeSize))
	{
		vertexShader->release();
		return nullptr;
	}

	return vertexShader;
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}

bool GraphicsEngine::compileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize)
{
	ID3DBlob* errblob = nullptr;
	HRESULT result = D3DCompileFromFile(fileName, nullptr, nullptr, entryPointName, "vs_5_0", 0, 0, &m_blob, &errblob);

	if (FAILED(result))
	{
		if (errblob)
		{
			errblob->Release();
		}

		return false;
	}

	*shaderByteCode = m_blob->GetBufferPointer();
	*byteCodeSize = m_blob->GetBufferSize();

	return true;
}

void GraphicsEngine::releaseCompiledShader()
{
	if (m_blob)
	{
		m_blob->Release();
	}
}

void GraphicsEngine::createShaders()
{
	ID3DBlob* errblob = nullptr;
	D3DCompileFromFile(L"TestShader.hlsl", nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &m_psblob, &errblob);
	m_d3dDevice->CreatePixelShader(m_psblob->GetBufferPointer(), m_psblob->GetBufferSize(), nullptr, &m_ps);
}

void GraphicsEngine::setShaders()
{
	m_immContext->PSSetShader(m_ps, nullptr, 0);
}