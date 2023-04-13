#pragma once
#include "d3d11.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "d3dcompiler.h"

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;
class PixelShader;

class GraphicsEngine
{
public:
	bool init();
	bool release();
	bool compileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize);
	bool compilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize);
	void releaseCompiledShader();
	SwapChain* createSwapChain();
	DeviceContext* getImmediateDeviceContext();
	VertexBuffer* createVertexBuffer();
	VertexShader* createVertexShader(const void* shaderByteCode, size_t byteCodeSize);
	PixelShader* createPixelShader(const void* shaderByteCode, size_t byteCodeSize);
public:
	static GraphicsEngine* get();
private:
	ID3D11Device* m_d3dDevice;
	D3D_FEATURE_LEVEL m_featureLevel;
	ID3D11DeviceContext* m_immContext;

	IDXGIDevice* m_dxgiDevice;
	IDXGIAdapter* m_dxgiAdapter;
	IDXGIFactory* m_dxgiFactory;

	DeviceContext* m_deviceContext;

	ID3DBlob* m_blob = nullptr;
	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;

	friend class SwapChain;
	friend class VertexBuffer;
	friend class VertexShader;
	friend class PixelShader;
};