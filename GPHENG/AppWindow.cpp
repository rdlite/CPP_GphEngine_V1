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

	FolderPathes pathes;

	m_skyboxTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(pathes.texturesFolder(L"sky.jpg").c_str());

	m_sandTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(pathes.texturesFolder(L"sand.jpg").c_str());
	m_barrelTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(pathes.texturesFolder(L"barrel.jpg").c_str());
	m_brickTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(pathes.texturesFolder(L"house_brick.jpg").c_str());
	m_windowTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(pathes.texturesFolder(L"house_windows.jpg").c_str());
	m_woodTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(pathes.texturesFolder(L"house_wood.jpg").c_str());

	m_skyMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(pathes.meshesFolder(L"sphere.obj").c_str());
	m_terrainMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(pathes.meshesFolder(L"terrain.obj").c_str());
	m_houseMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(pathes.meshesFolder(L"house.obj").c_str());

	RECT rc = this->getClientWindowRect();

	m_swapChain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(
		this->m_hwnd, (rc.right - rc.left), (rc.bottom - rc.top));

	m_worldCamera.setTranslation(Vector3(0, 0, -2.0f));

	m_terrainMat = GraphicsEngine::get()->createMaterial(L"TestShader.hlsl", L"TestShader.hlsl");
	m_terrainMat->addTexture(m_sandTex);
	m_terrainMat->setCullMode(CULL_BACK);

	m_brickMat = GraphicsEngine::get()->createMaterial(m_terrainMat);
	m_brickMat->addTexture(m_brickTex);
	m_brickMat->setCullMode(CULL_BACK);

	m_woodMat = GraphicsEngine::get()->createMaterial(m_brickMat);
	m_woodMat->addTexture(m_woodTex);
	m_woodMat->setCullMode(CULL_BACK);

	m_windowsMat = GraphicsEngine::get()->createMaterial(m_brickMat);
	m_windowsMat->addTexture(m_windowTex);
	m_windowsMat->setCullMode(CULL_BACK);

	m_barrelMat = GraphicsEngine::get()->createMaterial(m_brickMat);
	m_barrelMat->addTexture(m_barrelTex);
	m_barrelMat->setCullMode(CULL_BACK);

	m_skyboxMat = GraphicsEngine::get()->createMaterial(L"PointLightDemo.hlsl", L"SkyboxShader.hlsl");
	m_skyboxMat->addTexture(m_skyboxTex);
	m_skyboxMat->setCullMode(CULL_FRONT);

	m_isPlayState = true;
	m_isFullscreen = false;

	InputSystem::get()->setCursorVisible(!m_isPlayState);
	InputSystem::get()->setCursorPosition(Point(getWindowWidth() / 2.0f, getWindowHeight() / 2.0f));

	if (m_isFullscreen) 
	{
		RECT rc = this->getScreenSize();
		m_swapChain->setFullScreen(true, rc.right, rc.bottom);
	}

	m_listMaterials.reserve(32);
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

	m_listMaterials.clear();
	m_listMaterials.push_back(m_terrainMat);

	updateModel(Vector3(0, -1, 0), m_listMaterials);
	drawMesh(m_terrainMesh, m_listMaterials);

	int grid = 10;
	float distance = 10;

	for (int i = 0; i < grid; i++)
	{
		for (int j = 0; j < grid; j++)
		{
			m_listMaterials.clear();
			m_listMaterials.push_back(m_barrelMat);
			m_listMaterials.push_back(m_brickMat);
			m_listMaterials.push_back(m_windowsMat);
			m_listMaterials.push_back(m_woodMat);

			updateModel(Vector3((i - grid / 2) * distance, -1, (j - grid / 2) * distance), m_listMaterials);
			drawMesh(m_houseMesh, m_listMaterials);
		}
	}

	m_listMaterials.clear();
	m_listMaterials.push_back(m_skyboxMat);

	drawMesh(m_skyMesh, m_listMaterials);

	m_swapChain->present(true);
}

void AppWindow::update()
{
	m_deltaPos += m_deltaTime * 1.0f;
	
	updateCamera();
	updateLight();
	updateSkybox();
}

void AppWindow::updateModel(Vector3 position, const std::vector<MaterialPtr>& listMaterials)
{
	Constants cc;

	Matrix4x4 lightRotMatrix;
	lightRotMatrix.setIdentity();
	lightRotMatrix.setRotationX(-45.0f);
	lightRotMatrix.setRotationY(m_deltaPos * 0.2f);

	cc.m_world.setIdentity();
	cc.m_world.setTranslation(position);
	cc.m_view = m_viewCamera;
	cc.m_proj = m_projCamera;
	cc.m_cameraPosition = m_worldCamera.getTranslation();

	cc.m_lightPosition = m_lightPosition;
	cc.m_lightRadius = 4.0f;
	cc.m_lightDirection = lightRotMatrix.getForward();
	cc.m_time = m_time;

	for (size_t m = 0; m < listMaterials.size(); m++)
	{
		listMaterials[m]->setData(&cc, sizeof(Constants));
	}
}

void AppWindow::updateSkybox()
{
	Constants cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3(1.0f, 1.0f, 1.0f) * m_cameraFarPlaneDistance);
	cc.m_world.setTranslation(m_worldCamera.getTranslation());
	cc.m_view = m_viewCamera;
	cc.m_proj = m_projCamera;

	m_skyboxMat->setData(&cc, sizeof(Constants));
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
	//m_lightPosition = Vector3(cos(m_time) * 2.0f, 1.0f, sin(m_time) * 2.0f);
	m_lightPosition = Vector3(0.0f, -0.0f, 0.0f);
}

void AppWindow::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& listMaterials)
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	for (size_t m = 0; m < mesh->getNumMaterialSlots(); m++)
	{
		if (m >= listMaterials.size()) break;

		MaterialSlot mat = mesh->getMaterialSlot(m);
		GraphicsEngine::get()->getRenderSystem()->setRasterizerState(listMaterials[m]->getCullMode() == CULL_FRONT);
		GraphicsEngine::get()->setMaterial(listMaterials[m]);
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(
			mat.numIndicies, 0, mat.startIndex);
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