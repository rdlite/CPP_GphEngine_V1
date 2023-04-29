#pragma once
#include "RenderSystem.h"
#include "Prerequisites.h"
#include <exception>
#include "TextureManager.h"

class GraphicsEngine
{
private:
	GraphicsEngine();
	~GraphicsEngine();
public:
	RenderSystem* getRenderSystem();
	TextureManager* getTextureManager();
	static GraphicsEngine* get();
	static void create();
	static void release();
private:
	RenderSystem* m_renderSystem = nullptr;
	TextureManager* m_textureManager;
	static GraphicsEngine* m_engine;
};