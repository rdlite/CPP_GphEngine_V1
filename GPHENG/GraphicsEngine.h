#pragma once
#include "RenderSystem.h"
#include "Prerequisites.h"
#include <exception>
#include "TextureManager.h"
#include "MeshManager.h"
#include "DeviceContext.h"
#include "Material.h"

class GraphicsEngine
{
private:
	GraphicsEngine();
	~GraphicsEngine();
public:
	RenderSystem* getRenderSystem();
	TextureManager* getTextureManager();
	MeshManager* getMeshManager();
	static GraphicsEngine* get();
	static void create();
	static void release();
	void getVertexMeshLayoutShaderByteCodeAndSize(void** byteCode, size_t* size);
	MaterialPtr createMaterial(const wchar_t* VSPath, const wchar_t* PSPath);
	MaterialPtr createMaterial(const Material& material);
	void setMaterial(const Material& material);
private:
	RenderSystem* m_renderSystem = nullptr;
	TextureManager* m_textureManager = nullptr;
	MeshManager* m_meshManager = nullptr;
	static GraphicsEngine* m_engine;

	unsigned char m_meshLayoutByteCode[4096];
	size_t m_meshLayoutSize = 0;
};