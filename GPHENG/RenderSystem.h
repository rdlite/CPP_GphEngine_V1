#pragma once
#include "d3d11.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "d3dcompiler.h"
#include "Prerequisites.h"
#include <exception>

class RenderSystem
{
public:
	RenderSystem();
	bool compileVertexShader(
		const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, 
		size_t* byteCodeSize);
	bool compilePixelShader(
		const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, 
		size_t* byteCodeSize);
	void releaseCompiledShader();
	SwapChainPtr createSwapChain(
		HWND hwnd, UINT width, UINT height);
	DeviceContextPtr getImmediateDeviceContext();
	VertexBufferPtr createVertexBuffer(
		void* listVertices, UINT sizeVertex, UINT sizeList,
		void* shaderByteCode, UINT sizeByteShader);
	IndexBufferPtr createIndexBuffer(void* listIndicies, UINT sizeList);
	ConstantBufferPtr createConstantBuffer(void* buffer, UINT sizeBuffer);
	VertexShaderPtr createVertexShader(const void* shaderByteCode, size_t byteCodeSize);
	PixelShaderPtr createPixelShader(const void* shaderByteCode, size_t byteCodeSize);
	~RenderSystem();
private:
	ID3D11Device* m_d3dDevice;
	D3D_FEATURE_LEVEL m_featureLevel;
	ID3D11DeviceContext* m_immContext;

	IDXGIDevice* m_dxgiDevice;
	IDXGIAdapter* m_dxgiAdapter;
	IDXGIFactory* m_dxgiFactory;

	DeviceContextPtr m_deviceContext;

	ID3DBlob* m_blob = nullptr;
	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;

	friend class SwapChain;
	friend class VertexBuffer;
	friend class ConstantBuffer;
	friend class VertexShader;
	friend class IndexBuffer;
	friend class PixelShader;
	friend class Texture;
};