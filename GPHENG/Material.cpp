#include "Material.h"
#include <iostream>

Material::Material(const wchar_t* VSPath, const wchar_t* PSPath)
{
	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(VSPath, "vsmain", &shaderByteCode, &sizeShader);
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shaderByteCode, sizeShader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	if (!m_vs) throw std::runtime_error("Vertex material isnt loaded");

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(PSPath, "psmain", &shaderByteCode, &sizeShader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, sizeShader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	if (!m_ps) throw std::runtime_error("Pixel material isnt loaded");
}

Material::Material(const MaterialPtr& material)
{
	m_vs = material->m_vs;
	m_ps = material->m_ps;
}

void Material::addTexture(const TexturePtr& texture)
{
	m_textures.push_back(texture);
}

void Material::removeTexture(unsigned int index)
{
	if (index >= this->m_textures.size()) return;

	m_textures.erase(m_textures.begin() + index);
}

void Material::setData(void* data, unsigned int size)
{
	if (!m_cb)
	{
		m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(data, size);
	}
	else 
	{
		m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), data);
	}
}

void Material::setCullMode(CULL_MODE mode)
{
	m_cullMode = mode;
}

CULL_MODE Material::getCullMode()
{
	return m_cullMode;
}

Material::~Material()
{
}