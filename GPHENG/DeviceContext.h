#pragma once
#include "d3d11.h"
#include "SwapChain.h"

class SwapChain;
class VertexBuffer;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* deviceContext);
	void clearRenderTargetColor(SwapChain* swapChain, float r, float g, float b, float a);
	void setVertexBuffer(VertexBuffer* vertexBuffer);
	void drawTriangleList(UINT vertexCount, UINT startVertexIndex);
	void setViewportSize(UINT width, UINT height);
	bool release();
private:
	ID3D11DeviceContext* m_deviceContext;
};