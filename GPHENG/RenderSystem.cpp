#include "RenderSystem.h"
#include <iostream>

RenderSystem::RenderSystem()
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
		throw std::exception("Render system is not initialized");
	}

	m_deviceContext = std::make_shared<DeviceContext>(m_immContext, this);

	m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgiDevice);
	m_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgiAdapter);
	m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory);
}

SwapChainPtr RenderSystem::createSwapChain(
	HWND hwnd, UINT width, UINT height)
{
	SwapChainPtr sc = nullptr;
	
	try 
	{
		sc = std::make_shared<SwapChain>(hwnd, width, height, this);
	}
	catch (...) {}

	return sc;
}

DeviceContextPtr RenderSystem::getImmediateDeviceContext()
{
	return this->m_deviceContext;
}

VertexBufferPtr RenderSystem::createVertexBuffer(
	void* listVertices, UINT sizeVertex, UINT sizeList,
	void* shaderByteCode, UINT sizeByteShader)
{
	VertexBufferPtr vb = nullptr;

	try
	{
		vb = std::make_shared<VertexBuffer>(
			listVertices, sizeVertex, sizeList,
			shaderByteCode, sizeByteShader, this);
	}
	catch (...)  { }

	return vb;
}

IndexBufferPtr RenderSystem::createIndexBuffer(void* listIndicies, UINT sizeList)
{
	IndexBufferPtr ib = nullptr;

	try
	{
		ib = std::make_shared<IndexBuffer>(listIndicies, sizeList, this);
	}
	catch (...) {}

	return ib;
}

ConstantBufferPtr RenderSystem::createConstantBuffer(void* buffer, UINT sizeBuffer)
{
	ConstantBufferPtr cb = nullptr;

	try
	{
		cb = std::make_shared<ConstantBuffer>(buffer, sizeBuffer, this);
	}
	catch (...) {}

	return cb;
}

VertexShaderPtr RenderSystem::createVertexShader(const void* shaderByteCode, size_t byteCodeSize)
{
	VertexShaderPtr vertexShader = nullptr;

	try
	{
		vertexShader = std::make_shared<VertexShader>(shaderByteCode, byteCodeSize, this);
	}
	catch (...) {}

	return vertexShader;
}

PixelShaderPtr RenderSystem::createPixelShader(const void* shaderByteCode, size_t byteCodeSize)
{
	PixelShaderPtr pixelShader = nullptr;
	
	try
	{
		pixelShader = std::make_shared<PixelShader>(shaderByteCode, byteCodeSize, this);
	}
	catch (...) {}

	return pixelShader;
}

RenderSystem::~RenderSystem()
{
	if (m_vsblob) m_vsblob->Release();
	if (m_psblob) m_psblob->Release();

	m_dxgiDevice->Release();
	m_dxgiAdapter->Release();
	m_dxgiFactory->Release();

	m_d3dDevice->Release();
}

bool RenderSystem::compileVertexShader(
	const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, 
	size_t* byteCodeSize)
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

bool RenderSystem::compilePixelShader(
	const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, 
	size_t* byteCodeSize)
{
	ID3DBlob* errblob = nullptr;
	HRESULT result = D3DCompileFromFile(fileName, nullptr, nullptr, entryPointName, "ps_5_0", 0, 0, &m_blob, &errblob);

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

void RenderSystem::releaseCompiledShader()
{
	if (m_blob)
	{
		m_blob->Release();
	}
}