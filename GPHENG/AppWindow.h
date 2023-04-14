#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "InputListener.h"
#include "InputSystem.h"

class AppWindow: public Window, public InputListener
{
public:
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus(bool isFocus) override;

	void updateQuadPosition();

	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& deltaMousePos) override;
	virtual void onMouseButtonDown(int mouseButtonID, const Point& point) override;
	virtual void onMouseButtonUp(int mouseButtonID, const Point& point) override;
private:
	SwapChain* m_swapChain;
	VertexBuffer* m_vertexBuffer;
	IndexBuffer* m_indexBuffer;
	VertexShader* m_vertexShader;
	PixelShader* m_pixelShader;
	ConstantBuffer* m_constBuffer;

	float m_oldDelta, m_newDelta, m_deltaTime;
	float m_deltaPos;
	float m_scaleCube = 1.0f;

	float m_rotationX = 0.0f, m_rotationY = 0.0f;

	bool m_isLMouseButtonPress;
};