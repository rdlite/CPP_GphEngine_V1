#pragma once
#include "d3d11.h"
#include "SwapChain.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "Prerequisites.h"

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* deviceContext, RenderSystem* system);
	void clearRenderTargetColor(SwapChainPtr swapChain, float r, float g, float b, float a);

	void setVertexBuffer(VertexBufferPtr vertexBuffer);
	void setIndexBuffer(IndexBufferPtr indexBuffer);

	void setConstantBuffer(VertexShaderPtr shader, ConstantBufferPtr buffer);
	void setConstantBuffer(PixelShaderPtr shader, ConstantBufferPtr buffer);

	void drawTriangleList(UINT vertexCount, UINT startVertexIndex);
	void drawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation);
	void drawTriangleStrip(UINT vertexCount, UINT startVertexIndex);

	void setViewportSize(UINT width, UINT height);

	void setVertexShader(VertexShaderPtr vertexShader);
	void setPixelShader(PixelShaderPtr pixelShader);

	~DeviceContext();
private:
	RenderSystem* m_system = nullptr;

	ID3D11DeviceContext* m_deviceContext;

	friend class ConstantBuffer;
};