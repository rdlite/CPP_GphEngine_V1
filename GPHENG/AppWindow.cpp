#include "AppWindow.h"
#include "iostream"

void AppWindow::onCreate() 
{
	Window::onCreate();

	//m_woodTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\wood.jpg");
	m_woodTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick.png");
	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\monke.obj");

	RECT rc = this->getClientWindowRect();

	m_swapChain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(
		this->m_hwnd, (rc.right - rc.left), (rc.bottom - rc.top));

	m_worldCamera.setTranslation(Vector3(0, 0, -1.5f));

	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"TestShader.hlsl", "vsmain", &shaderByteCode, &sizeShader);
	m_vertexShader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shaderByteCode, sizeShader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"TestShader.hlsl", "psmain", &shaderByteCode, &sizeShader);
	m_pixelShader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, sizeShader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	Constants cc;
	
	m_constBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(Constants));

	InputSystem::get()->setCursorVisible(false);
	InputSystem::get()->setCursorPosition(Point(getWindowWidth() / 2.0f, getWindowHeight() / 2.0f));
}

void AppWindow::onUpdate() 
{
	Window::onUpdate();

	InputSystem::get()->update();

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swapChain, .0f, .3f, .4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize((rc.right - rc.left), (rc.bottom - rc.top));

	update();

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vertexShader, m_constBuffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_pixelShader, m_constBuffer);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vertexShader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_pixelShader);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_pixelShader, m_woodTexture);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_mesh->getVertexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_mesh->getIndexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);

	m_swapChain->present(true);

	m_oldDelta = m_newDelta;
	m_newDelta = GetTickCount64();
	m_deltaTime = (m_oldDelta) ? (m_newDelta - m_oldDelta) / 1000.0f : 0;
}

void AppWindow::update()
{
	Constants cc;

	m_deltaPos += m_deltaTime * 1.5f;

	Matrix4x4 temp;
	Matrix4x4 lightRotMatrix;
	lightRotMatrix.setIdentity();
	lightRotMatrix.setRotationY(m_deltaPos);

	cc.m_lightDirection = lightRotMatrix.getForward();

	//cc.World.setScale(Vector3::lerp(Vector3(0.5f, 0.5f, 1.0f), Vector3(1.0f, 1.0f, 1.0f), abs(sin(m_deltaPos))));
	//cc.World *= temp;

	cc.m_world.setScale(Vector3(1.0f, 1.0f, 1.0f) * m_scaleCube);

	cc.m_world.setIdentity();

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
	cc.m_cameraPosition = newPos;
	m_worldCamera = worldCam;

	worldCam.inverse();

	cc.m_view = worldCam;
	//cc.Proj.setOrthoLH(
	//	(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
	//	(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
	//	-4.0f,
	//	4.0f
	//);

	int width = getWindowWidth();
	int height = getWindowHeight();

	cc.m_proj.setPerspective(
		1.5f, (float)(width / height), .1f, 100.0f
	);

	m_constBuffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onKeyDown(int key)
{
	float rotSpeed = 0.6f;

	if (key == 'W')
	{
		m_forward = 1.0f * rotSpeed;
	}
	else if (key == 'S')
	{
		m_forward = -1.0f * rotSpeed;
	}
	else if (key == 'A')
	{
		m_right = -1.0f * rotSpeed;
	}
	else if (key == 'D')
	{
		m_right = 1.0f * rotSpeed;
	}
	else if (key == 'D')
	{
		m_right = 1.0f * rotSpeed;
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
	float rotSpeed = .15f;

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
	if (isFocus && !m_isFocused)
	{
		m_isFocused = true;
		InputSystem::get()->addListener(this);
	}
	else if (!isFocus && m_isFocused)
	{
		m_isFocused = false;
		InputSystem::get()->removeListener(this);
	}
}

void AppWindow::onDestroy() 
{
	Window::onDestroy();
}