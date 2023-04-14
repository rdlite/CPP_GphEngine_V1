#include "AppWindow.h"

void AppWindow::onCreate() 
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swapChain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swapChain->init(this->m_hwnd, (rc.right - rc.left), (rc.bottom - rc.top));

	m_worldCamera.setTranslation(Vector3(0, 0, -2));

	Vertex vertexList[] = 
	{
		{Vector3(-0.5f, -0.5f, -0.5f),		Color(0, 0, 0)},
		{Vector3(-0.5f, 0.5f, -0.5f),		Color(0, 0, 1)},
		{Vector3(0.5f, 0.5f, -0.5f),		Color(0, 1, 0)},
		{Vector3(0.5f, -0.5f, -0.5f),		Color(0, 1, 1)},

		{Vector3(0.5f, -0.5f, 0.5f),		Color(1, 0, 0)},
		{Vector3(0.5f, 0.5f, 0.5f),			Color(1, 0, 1)},
		{Vector3(-0.5f, 0.5f, 0.5f),		Color(1, 1, 0)},
		{Vector3(-0.5f, -0.5f, 0.5f),		Color(1, 1, 1)},
	};

	m_vertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	UINT sizeVertexList = ARRAYSIZE(vertexList);

	unsigned int indexList[] =
	{
		0, 1, 2,	2, 3, 0,
		4, 5, 6,	6, 7, 4,
		1, 6, 5,	5, 2, 1,
		7, 0, 3,	3, 4, 7,
		3, 2, 5,	5, 4, 3,
		7, 6, 1,	1, 0, 7,
	};

	m_indexBuffer = GraphicsEngine::get()->createIndexBuffer();
	UINT sizeIndexList = ARRAYSIZE(indexList);
	m_indexBuffer->load(indexList, sizeIndexList);

	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;

	GraphicsEngine::get()->compileVertexShader(L"TestShader.hlsl", "vsmain", &shaderByteCode, &sizeShader);
	m_vertexShader = GraphicsEngine::get()->createVertexShader(shaderByteCode, sizeShader);

	m_vertexBuffer->load(vertexList, sizeof(Vertex), sizeVertexList, shaderByteCode, sizeShader);

	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"TestShader.hlsl", "psmain", &shaderByteCode, &sizeShader);
	m_pixelShader = GraphicsEngine::get()->createPixelShader(shaderByteCode, sizeShader);

	GraphicsEngine::get()->releaseCompiledShader();

	Constants cc;
	cc.Time = 0;

	m_constBuffer = GraphicsEngine::get()->createConstantBuffer();
	m_constBuffer->load(&cc, sizeof(Constants));

	InputSystem::get()->setCursorVisible(false);
	InputSystem::get()->setCursorPosition(Point(getWindowWidth() / 2.0f, getWindowHeight() / 2.0f));
}

void AppWindow::onUpdate() 
{
	Window::onUpdate();

	InputSystem::get()->update();

	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swapChain, .0f, .3f, .4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize((rc.right - rc.left), (rc.bottom - rc.top));

	update();

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vertexShader, m_constBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_pixelShader, m_constBuffer);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vertexShader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_pixelShader);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vertexBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_indexBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_indexBuffer->getSizeIndexList(), 0, 0);

	m_swapChain->present(false);

	m_oldDelta = m_newDelta;
	m_newDelta = GetTickCount64();
	m_deltaTime = (m_oldDelta) ? (m_newDelta - m_oldDelta) / 1000.0f : 0;
}

void AppWindow::update()
{
	Constants cc;
	cc.Time = GetTickCount64();

	m_deltaPos += m_deltaTime * .5f;

	Matrix4x4 temp;

	//cc.World.setScale(Vector3::lerp(Vector3(0.5f, 0.5f, 1.0f), Vector3(1.0f, 1.0f, 1.0f), abs(sin(m_deltaPos))));
	//cc.World *= temp;

	cc.World.setScale(Vector3(1.0f, 1.0f, 1.0f) * m_scaleCube);

	cc.World.setIdentity();

	Matrix4x4 worldCam;
	worldCam.setIdentity();
	temp.setIdentity();
	temp.setRotationX(m_rotationX);	
	worldCam *= temp;
	temp.setIdentity();
	temp.setRotationY(m_rotationY);
	worldCam *= temp;

	float flyingSpeed = .1f;

	Vector3 newPos = m_worldCamera.getTranslation() + worldCam.getForward() * (m_forward * flyingSpeed) + worldCam.getRight() * (m_right * flyingSpeed);

	worldCam.setTranslation(newPos);
	m_worldCamera = worldCam;

	worldCam.inverse();

	cc.View = worldCam;
	//cc.Proj.setOrthoLH(
	//	(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
	//	(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
	//	-4.0f,
	//	4.0f
	//);

	int width = getWindowWidth();
	int height = getWindowHeight();

	cc.Proj.setPerspective(
		1.5f, (float)(width / height), .1f, 100.0f
	);

	m_constBuffer->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onKeyDown(int key)
{
	float rotSpeed = 4.0f;

	if (key == 'W')
	{
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		m_right = -1.0f;
	}
	else if (key == 'D')
	{
		m_right = 1.0f;
	}
	else if (key == 'D')
	{
		m_right = 1.0f;
	}
	else if (key == 27)
	{
		//CloseWindow(m_hwnd);
	}
}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_right = 0.0f;
}

void AppWindow::onMouseMove(const Point& mousePos)
{
	float rotSpeed = .25f;

	m_rotationX += rotSpeed * m_deltaTime * (mousePos.y - (getWindowHeight() / 2.0f));
	m_rotationY -= rotSpeed * m_deltaTime * (mousePos.x - (getWindowWidth() / 2.0f));

	InputSystem::get()->setCursorPosition(Point(getWindowWidth() / 2.0f, getWindowHeight() / 2.0f));
}

void AppWindow::onMouseButtonDown(int mouseButtonID, const Point& point)
{
	if (mouseButtonID == 0)
	{
		m_isLMouseButtonPress = true;
	}
}

void AppWindow::onMouseButtonUp(int mouseButtonID, const Point& point)
{
	if (mouseButtonID == 0)
	{
		m_isLMouseButtonPress = false;
	}
}

float AppWindow::getWindowWidth()
{
	return (this->getClientWindowRect().right - this->getClientWindowRect().left);
}

float AppWindow::getWindowHeight()
{
	return (this->getClientWindowRect().bottom - this->getClientWindowRect().top);
}

void AppWindow::onFocus(bool isFocus)
{
	if (isFocus)
	{
		InputSystem::get()->addListener(this);
	}
	else 
	{
		InputSystem::get()->removeListener(this);
	}
}

void AppWindow::onDestroy() 
{
	Window::onDestroy();
	m_vertexBuffer->release();
	m_indexBuffer->release();
	m_constBuffer->release();
	m_swapChain->release();
	m_pixelShader->release();
	m_vertexShader->release();
	GraphicsEngine::get()->release();
}