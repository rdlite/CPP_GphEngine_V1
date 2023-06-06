#pragma once
#include "Window.h"
#include "FolderPathes.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "InputListener.h"
#include "InputSystem.h"
#include "Math.h"
#include "Mesh.h"
#include "Windows.h"
#include <cmath>

class AppWindow : public Window, public InputListener
{
public:
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus(bool isFocus) override;
	virtual void onSize() override;

	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mousePos) override;
	virtual void onMouseButtonDown(int mouseButtonID, const Point& point) override;
	virtual void onMouseButtonUp(int mouseButtonID, const Point& point) override;
public:
	void render();
	void update();
	void updateModel(Vector3 position, Vector3 rotation, const std::vector<MaterialPtr>& listMaterials);
	void updateSkybox();
	void updateCamera();
	void updateLight();
	void drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& listMaterials);
private:
	float getWindowWidth();
	float getWindowHeight();

	SwapChainPtr m_swapChain;
	ConstantBufferPtr m_modelCB;
	ConstantBufferPtr m_skyboxCB;

	TexturePtr m_skyboxTex;
	TexturePtr m_sandTex;
	TexturePtr m_barrelTex;
	TexturePtr m_brickTex;
	TexturePtr m_windowTex;
	TexturePtr m_woodTex;

	MeshPtr m_skyMesh;
	MeshPtr m_terrainMesh;
	MeshPtr m_houseMesh;

	MaterialPtr m_skyboxMat;
	MaterialPtr m_terrainMat;
	MaterialPtr m_barrelMat;
	MaterialPtr m_brickMat;
	MaterialPtr m_windowsMat;
	MaterialPtr m_woodMat;

	Matrix4x4 m_worldCamera;
	Matrix4x4 m_viewCamera;
	Matrix4x4 m_projCamera;

	std::vector<MaterialPtr> m_listMaterials;

	Vector4 m_lightPosition;

	long double m_oldDelta = 0.0f, m_newDelta = 0.0f, m_deltaTime = 0.0f;
	float m_deltaPos = 0.0f;
	float m_forward = 0.0f, m_right = 0.0f;
	long double m_time = 0.0f;

	float m_cameraFarPlaneDistance = 100.0f;

	float m_rotationX = 0.0f, m_rotationY = 0.0f;

	bool m_isLMouseButtonPress = false;
	bool m_isPlayState = false;
	bool m_isFullscreen = false;
};