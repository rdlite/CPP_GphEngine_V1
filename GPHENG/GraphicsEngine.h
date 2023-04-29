#pragma once
#include "RenderSystem.h"
#include "Prerequisites.h"
#include <exception>

class GraphicsEngine
{
private:
	GraphicsEngine();
	~GraphicsEngine();
public:
	RenderSystem* getRenderSystem();
	static GraphicsEngine* get();
	static void create();
	static void release();
private:
	RenderSystem* m_render_system = nullptr;
	static GraphicsEngine* m_engine;
};