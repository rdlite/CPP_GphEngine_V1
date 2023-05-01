#include "AppWindow.h"
#include "iostream"

__declspec(align(16))
struct Constants
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4 m_lightDirection;
	Vector4 m_cameraPosition;
	Vector4 m_lightPosition = Vector4(0, 1, 0, 0);
	float m_lightRadius = 4.0f;
	float m_time = 0.0f;
};

void AppWindow::onCreate() 
{
	Window::onCreate();

	m_wallTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\wall.jpg");
	m_bricksTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick.png");
	m_earthTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_color.jpg");;

	m_skyboxTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stars_map.jpg");

	m_shpereMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	m_torusMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\torus.obj");
	m_monkeMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\monke.obj");
	m_skyMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");

	RECT rc = this->getClientWindowRect();

	m_swapChain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(
		this->m_hwnd, (rc.right - rc.left), (rc.bottom - rc.top));

	m_worldCamera.setTranslation(Vector3(0, 0, -2.0f));

	m_wallMat = GraphicsEngine::get()->createMaterial(L"TestShader.hlsl", L"TestShader.hlsl");
	m_wallMat->addTexture(m_wallTexture);
	m_wallMat->setCullMode(CULL_BACK);

	m_bricksMat = GraphicsEngine::get()->createMaterial(m_wallMat);
	m_bricksMat->addTexture(m_bricksTexture);
	m_bricksMat->setCullMode(CULL_BACK);

	m_earthMat = GraphicsEngine::get()->createMaterial(m_bricksMat);
	m_earthMat->addTexture(m_earthTexture);
	m_earthMat->setCullMode(CULL_BACK);

	m_skyboxMaterial = GraphicsEngine::get()->createMaterial(L"PointLightDemo.hlsl", L"SkyboxShader.hlsl");
	m_skyboxMaterial->addTexture(m_skyboxTexture);
	m_skyboxMaterial->setCullMode(CULL_FRONT);

	m_isPlayState = true;
	m_isFullscreen = false;

	InputSystem::get()->setCursorVisible(!m_isPlayState);
	InputSystem::get()->setCursorPosition(Point(getWindowWidth() / 2.0f, getWindowHeight() / 2.0f));

	if (m_isFullscreen) 
	{
		RECT rc = this->getScreenSize();
		m_swapChain->setFullScreen(true, rc.right, rc.bottom);
	}
}

void AppWindow::onUpdate() 
{
	m_oldDelta = m_newDelta;
	m_newDelta = GetTickCount64();
	m_deltaTime = (m_oldDelta) ? (m_newDelta - m_oldDelta) / 1000.0f : 0;
	m_time += m_deltaTime;

	Window::onUpdate();
	InputSystem::get()->update();
	this->render();
}

void AppWindow::render()
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swapChain, .0f, .3f, .4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize((rc.right - rc.left), (rc.bottom - rc.top));

	update();

	updateModel(Vector3(-4, 0, 1 * i), m_wallMat);
	drawMesh(m_shpereMesh, m_wallMat);

	updateModel(Vector3(0, 0, 1 * i), m_bricksMat);
	drawMesh(m_torusMesh, m_bricksMat);

	updateModel(Vector3(4, 0, 1 * i), m_earthMat);
	drawMesh(m_monkeMesh, m_earthMat);

	drawMesh(m_skyMesh, m_skyboxMaterial);

	m_swapChain->present(true);
}

void AppWindow::update()
{
	m_deltaPos += m_deltaTime * 1.0f;
	
	updateCamera();
	updateLight();
	updateSkybox();
	//updateModel(Vector3(0, 0, 0), m_meshMaterial);
}

void AppWindow::updateModel(Vector3 position, const MaterialPtr& material)
{
	Constants cc;

	Matrix4x4 lightRotMatrix;
	lightRotMatrix.setIdentity();
	lightRotMatrix.setRotationY(-m_deltaPos / 2.0f);

	cc.m_world.setIdentity();
	cc.m_world.setTranslation(position);
	cc.m_view = m_viewCamera;
	cc.m_proj = m_projCamera;
	cc.m_cameraPosition = m_worldCamera.getTranslation();

	cc.m_lightPosition = m_lightPosition;
	cc.m_lightRadius = 4.0f;
	cc.m_lightDirection = lightRotMatrix.getForward();
	cc.m_time = m_time;

	material->setData(&cc, sizeof(Constants));
}

void AppWindow::updateSkybox()
{
	Constants cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3(1.0f, 1.0f, 1.0f) * m_cameraFarPlaneDistance);
	cc.m_world.setTranslation(m_worldCamera.getTranslation());
	cc.m_view = m_viewCamera;
	cc.m_proj = m_projCamera;

	m_skyboxMaterial->setData(&cc, sizeof(Constants));
}

bool isShiftPressed = false;

void AppWindow::onKeyDown(int key)
{
	float shiftSpeed = 4.0f;

	float rotSpeed = 0.6f * (isShiftPressed ? shiftSpeed : 1.0f);

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
	else if (key == VK_SHIFT)
	{
		isShiftPressed = true;
	}
}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_right = 0.0f;

	if (key == 'G')
	{
		m_isPlayState = !m_isPlayState;

		InputSystem::get()->setCursorVisible(!m_isPlayState);
	}
	else if (key == 'F')
	{
		m_isFullscreen = !m_isFullscreen;

		InputSystem::get()->setCursorVisible(!m_isPlayState);

		RECT rc = this->getScreenSize();
		m_swapChain->setFullScreen(m_isFullscreen, rc.right, rc.bottom);
	}
	else if (key == VK_SHIFT)
	{
		isShiftPressed = false;
	}
}

void AppWindow::onMouseMove(const Point& mousePos)
{
	if (!m_isPlayState) return;

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

void AppWindow::updateCamera()
{
	Matrix4x4 worldCam, temp;
	worldCam.setIdentity();
	temp.setIdentity();
	temp.setRotationX(m_rotationX);
	worldCam *= temp;
	temp.setIdentity();
	temp.setRotationY(m_rotationY);
	worldCam *= temp;

	float flyingSpeed = .025f;

	Vector3 newPos = m_worldCamera.getTranslation() + worldCam.getForward() * (m_forward * flyingSpeed) + worldCam.getRight() * (m_right * flyingSpeed);

	worldCam.setTranslation(newPos);
	m_worldCamera = worldCam;

	worldCam.inverse();

	m_viewCamera = worldCam;

	int width = getWindowWidth();
	int height = getWindowHeight();

	m_projCamera.setPerspective(
		1.5f, ((float)width / (float)height), .01f, m_cameraFarPlaneDistance
	);
	//cc.Proj.setOrthoLH(
	//	(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
	//	(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
	//	-4.0f,
	//	4.0f
	//);
}

void AppWindow::updateLight()
{
	m_lightPosition = Vector3(cos(m_time) * 2.0f, 1.0f, sin(m_time) * 2.0f);
}

void AppWindow::drawMesh(const MeshPtr& mesh, const MaterialPtr& material)
{
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(material->getCullMode() == CULL_FRONT);

	GraphicsEngine::get()->setMaterial(*material);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(
		mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
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

void AppWindow::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swapChain->resize(rc.right, rc.bottom);
	this->render();
}

void AppWindow::onDestroy() 
{
	Window::onDestroy();
	m_swapChain->setFullScreen(false, 0, 0);
}