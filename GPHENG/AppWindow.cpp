#include "AppWindow.h"
#include "Math.h"

void AppWindow::onCreate() 
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swapChain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swapChain->init(this->m_hwnd, (rc.right - rc.left), (rc.bottom - rc.top));

	Vertex list[] =
	{
		{-0.5f, -0.5f, 0.0f},
		{0.0f, 0.5f, 0.0f},
		{0.5f, -0.5f, 0.0f},
	};

	m_vertexBuffer = GraphicsEngine::get()->createVertexBuffer();

	UINT sizeList = ARRAYSIZE(list);

	GraphicsEngine::get()->createShaders();

	void* shaderByteCode = nullptr;
	UINT sizeShader = 0;
	GraphicsEngine::get()->getShaderBufferAndSize(&shaderByteCode, &sizeShader);

	m_vertexBuffer->load(list, sizeof(Vertex), sizeList, shaderByteCode, sizeShader);
}

void AppWindow::onUpdate() 
{
	Window::onUpdate();
	
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swapChain, .0f, .3f, .4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize((rc.right - rc.left), (rc.bottom - rc.top));
	GraphicsEngine::get()->setShaders();

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vertexBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vertexBuffer->getSizeVertexList(), 0);

	m_swapChain->present(false);
}

void AppWindow::onDestroy() 
{
	Window::onDestroy();
	m_vertexBuffer->release();
	m_swapChain->release();
	GraphicsEngine::get()->release();
}