#include "AppWindow.h"
#include "Math.h"
#include "Structs.h"
#include "Windows.h"

void AppWindow::onCreate() 
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swapChain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swapChain->init(this->m_hwnd, (rc.right - rc.left), (rc.bottom - rc.top));

	Vertex list[] =
	{
		{-0.5f, -0.5f, 0.0f,	1, 0, 0},
		{-0.5f, 0.5f, 0.0f,		0, 1, 0},
		{0.5f, -0.5f, 0.0f,		0, 0, 1},
		{0.5f, 0.5f, 0.0f,		1, 1, 0},
	};

	m_vertexBuffer = GraphicsEngine::get()->createVertexBuffer();

	UINT sizeList = ARRAYSIZE(list);

	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;

	GraphicsEngine::get()->compileVertexShader(L"TestShader.hlsl", "vsmain", &shaderByteCode, &sizeShader);
	m_vertexShader = GraphicsEngine::get()->createVertexShader(shaderByteCode, sizeShader);

	m_vertexBuffer->load(list, sizeof(Vertex), sizeList, shaderByteCode, sizeShader);
	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"TestShader.hlsl", "psmain", &shaderByteCode, &sizeShader);
	m_pixelShader = GraphicsEngine::get()->createPixelShader(shaderByteCode, sizeShader);

	GraphicsEngine::get()->releaseCompiledShader();

	Constants cc;
	cc.Time = 0;

	m_constBuffer = GraphicsEngine::get()->createConstantBuffer();
	m_constBuffer->load(&cc, sizeof(Constants));
}

void AppWindow::onUpdate() 
{
	Window::onUpdate();
	
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swapChain, .0f, .3f, .4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize((rc.right - rc.left), (rc.bottom - rc.top));

	Constants cc;
	cc.Time = GetTickCount64();
	m_constBuffer->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vertexShader, m_constBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_pixelShader, m_constBuffer);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vertexShader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_pixelShader);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vertexBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vertexBuffer->getSizeVertexList(), 0);

	m_swapChain->present(false);
}

void AppWindow::onDestroy() 
{
	Window::onDestroy();
	m_vertexBuffer->release();
	m_swapChain->release();
	m_pixelShader->release();
	m_vertexShader->release();
	GraphicsEngine::get()->release();
}