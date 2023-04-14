#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"

class AppWindow: public Window
{
public:
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	void updateQuadPosition();
private:
	SwapChain* m_swapChain;
	VertexBuffer* m_vertexBuffer;
	IndexBuffer* m_indexBuffer;
	VertexShader* m_vertexShader;
	PixelShader* m_pixelShader;
	ConstantBuffer* m_constBuffer;

	float m_oldDelta, m_newDelta, m_deltaTime;
	float m_deltaPos;
};