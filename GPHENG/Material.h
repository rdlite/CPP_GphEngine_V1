#pragma once
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicsEngine.h"
#include "ConstantBuffer.h"
#include <exception>
#include <vector>

enum CULL_MODE
{
	NO_CULL = 0,
	CULL_BACK = 1,
	CULL_FRONT = 2,
};

class GraphicsEngine;

class Material
{
public:
	Material(const wchar_t* VSPath, const wchar_t* PSPath);
	Material(const MaterialPtr& material);
	void addTexture(const TexturePtr& texture);
	void removeTexture(unsigned int index);
	void setData(void* data, unsigned int size);
	void setCullMode(CULL_MODE mode);
	CULL_MODE getCullMode();
	~Material();
private:
	std::vector<TexturePtr> m_textures;
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	ConstantBufferPtr m_cb;
	CULL_MODE m_cullMode = CULL_BACK;

	friend class GraphicsEngine;
};