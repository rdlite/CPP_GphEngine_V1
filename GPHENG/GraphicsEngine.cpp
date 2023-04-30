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

	try
	{
		m_meshManager = new MeshManager();
	}
	catch (std::exception e)
	{
		throw e;
	}

	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;

	m_renderSystem->compileVertexShader(
		L"VertexMeshLayoutShader.hlsl", "vsmain", &shaderByteCode, 
		&sizeShader);
	::memcpy(m_meshLayoutByteCode, shaderByteCode, sizeShader);
	m_meshLayoutSize = sizeShader;
	m_renderSystem->releaseCompiledShader();
}

GraphicsEngine::~GraphicsEngine()
{
	delete m_meshManager;
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

MeshManager* GraphicsEngine::getMeshManager()
{
	return m_meshManager;
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

void GraphicsEngine::getVertexMeshLayoutShaderByteCodeAndSize(void** byteCode, size_t* size)
{
	*byteCode = m_meshLayoutByteCode;
	*size = m_meshLayoutSize;
}