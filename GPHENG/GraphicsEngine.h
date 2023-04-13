#pragma once
#include "d3d11.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "d3dcompiler.h"

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;

class GraphicsEngine
{
public:
	bool init();
	bool release();
	bool compileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize);
	void releaseCompiledShader();
	void createShaders();
	void setShaders();
	SwapChain* createSwapChain();
	DeviceContext* getImmediateDeviceContext();
	VertexBuffer* createVertexBuffer();
	VertexShader* createVertexShader(const void* shaderByteCode, size_t byteCodeSize);
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
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;

	friend class SwapChain;
	friend class VertexBuffer;
	friend class VertexShader;
};