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
	float m_time = 0.0f;
};

void AppWindow::onCreate() 
{
	Window::onCreate();

	m_earthColorTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_color.jpg");
	m_earthSpecularTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_spec.jpg");
	m_earthCloudsTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\clouds.jpg");
	m_earthNightTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_night.jpg");
	m_skyboxTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stars_map.jpg");

	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere_hq.obj");
	m_skyMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");

	RECT rc = this->getClientWindowRect();

	m_swapChain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(
		this->m_hwnd, (rc.right - rc.left), (rc.bottom - rc.top));

	m_worldCamera.setTranslation(Vector3(0, 0, -1.5f));

	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"TestShader.hlsl", "vsmain", &shaderByteCode, &sizeShader);
	m_VS = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shaderByteCode, sizeShader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"TestShader.hlsl", "psmain", &shaderByteCode, &sizeShader);
	m_PS = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, sizeShader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"SkyboxShader.hlsl", "psmain", &shaderByteCode, &sizeShader);
	m_skyboxPS = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, sizeShader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	Constants cc;
	
	m_modelCB = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(Constants));
	m_skyboxCB = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(Constants));

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

	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(false);

	TexturePtr listTex[4];
	listTex[0] = m_earthColorTexture;
	listTex[1] = m_earthSpecularTexture;
	listTex[2] = m_earthCloudsTexture;
	listTex[3] = m_earthNightTexture;

	drawMesh(
		m_mesh, m_VS, m_PS,
		m_modelCB, listTex, (unsigned int)ARRAYSIZE(listTex));

	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(true);

	drawMesh(
		m_skyMesh, m_VS, m_skyboxPS,
		m_skyboxCB, &m_skyboxTexture, 1);

	m_swapChain->present(true);
}

void AppWindow::update()
{
	m_deltaPos += m_deltaTime * 1.0f;

	updateCamera();
	updateModel();
	updateSkybox();
}

void AppWindow::updateModel()
{
	Constants cc;

	Matrix4x4 lightRotMatrix;
	lightRotMatrix.setIdentity();
	lightRotMatrix.setRotationY(-m_deltaPos / 2.0f);

	cc.m_world.setIdentity();
	cc.m_view = m_viewCamera;
	cc.m_proj = m_projCamera;
	cc.m_cameraPosition = m_worldCamera.getTranslation();
	cc.m_lightDirection = lightRotMatrix.getForward();
	cc.m_time = m_time;

	m_modelCB->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::updateSkybox()
{
	Constants cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3(1.0f, 1.0f, 1.0f) * m_cameraFarPlaneDistance);
	cc.m_world.setTranslation(m_worldCamera.getTranslation());
	cc.m_view = m_viewCamera;
	cc.m_proj = m_projCamera;

	m_skyboxCB->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
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

	float flyingSpeed = .1f;

	Vector3 newPos = m_worldCamera.getTranslation() + worldCam.getForward() * (m_forward * flyingSpeed) + worldCam.getRight() * (m_right * flyingSpeed);

	worldCam.setTranslation(newPos);
	m_worldCamera = worldCam;

	worldCam.inverse();

	m_viewCamera = worldCam;

	int width = getWindowWidth();
	int height = getWindowHeight();

	m_projCamera.setPerspective(
		1.5f, ((float)width / (float)height), .1f, m_cameraFarPlaneDistance
	);
	//cc.Proj.setOrthoLH(
	//	(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
	//	(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
	//	-4.0f,
	//	4.0f
	//);
}

void AppWindow::drawMesh(const MeshPtr& mesh, const VertexShaderPtr& vertexShader, const PixelShaderPtr& pixelShader,
	const ConstantBufferPtr& constBuffer, const TexturePtr* textures, unsigned int texNums)
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(vertexShader, constBuffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(pixelShader, constBuffer);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(vertexShader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(pixelShader);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(pixelShader, textures, texNums);

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