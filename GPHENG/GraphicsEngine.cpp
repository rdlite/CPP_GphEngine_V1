#include "GraphicsEngine.h"
#include "iostream"
#include "FolderPathes.h"

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

	FolderPathes pathes;

	m_renderSystem->compileVertexShader(
		pathes.shadersFolder(L"VertexMeshLayoutShader.hlsl").c_str(), "vsmain", &shaderByteCode,
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

MaterialPtr GraphicsEngine::createMaterial(const wchar_t* VSPath, const wchar_t* PSPath)
{
	MaterialPtr material = nullptr;

	try
	{
		material = std::make_shared<Material>(VSPath, PSPath);
	}
	catch (...) {}

	return material;
}

MaterialPtr GraphicsEngine::createMaterial(const Material& material)
{
	MaterialPtr newmaterial = nullptr;

	try
	{
		newmaterial = std::make_shared<Material>(material);
	}
	catch (...) {}

	return newmaterial;
}

void GraphicsEngine::setMaterial(const MaterialPtr& material)
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(material->m_vs, material->m_cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(material->m_ps, material->m_cb);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(material->m_vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(material->m_ps);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(
		material->m_ps,
		&material->m_textures[0],
		material->m_textures.size());
}