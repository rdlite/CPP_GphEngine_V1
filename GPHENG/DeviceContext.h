#pragma once
#include "d3d11.h"
#include "SwapChain.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Prerequisites.h"

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* deviceContext, RenderSystem* system);
	void clearRenderTargetColor(SwapChainPtr swapChain, float r, float g, float b, float a);

	void setVertexBuffer(const VertexBufferPtr& vertexBuffer);
	void setIndexBuffer(const IndexBufferPtr& indexBuffer);

	void setConstantBuffer(const VertexShaderPtr& shader, const ConstantBufferPtr& buffer);
	void setConstantBuffer(const PixelShaderPtr& shader, const ConstantBufferPtr& buffer);

	void drawTriangleList(UINT vertexCount, UINT startVertexIndex);
	void drawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation);
	void drawTriangleStrip(UINT vertexCount, UINT startVertexIndex);

	void setViewportSize(UINT width, UINT height);

	void setVertexShader(const VertexShaderPtr& vertexShader);
	void setPixelShader(const PixelShaderPtr& pixelShader);

	void setTexture(const VertexShaderPtr& vertexShader, const TexturePtr& texture);
	void setTexture(const PixelShaderPtr& pixelShader, const TexturePtr& texture);

	~DeviceContext();
private:
	RenderSystem* m_system = nullptr;

	ID3D11DeviceContext* m_deviceContext;

	friend class ConstantBuffer;
};