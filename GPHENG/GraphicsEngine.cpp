#include "GraphicsEngine.h"

GraphicsEngine* GraphicsEngine::m_engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	try
	{
		m_renderSystem = new RenderSystem();
	}
	catch (std::exception e)
	{
		throw e;
	}

	try
	{
		m_textureManager = new TextureManager();
	}
	catch (std::exception e)
	{
		throw e;
	}
}

GraphicsEngine::~GraphicsEngine()
{
	delete m_textureManager;
	delete m_renderSystem;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return m_renderSystem;
}

TextureManager* GraphicsEngine::getTextureManager()
{
	return m_textureManager;
}

GraphicsEngine* GraphicsEngine::get()
{
	return m_engine;
}

void GraphicsEngine::create()
{
	if (GraphicsEngine::m_engine) throw std::exception("Graphics engine already created");
	GraphicsEngine::m_engine = new GraphicsEngine();
}

void GraphicsEngine::release()
{
	if (!GraphicsEngine::m_engine) return;
	delete GraphicsEngine::m_engine;
}