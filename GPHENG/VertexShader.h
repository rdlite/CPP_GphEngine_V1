#pragma once
#include "GraphicsEngine.h"
#include "d3d11.h"
#include "Prerequisites.h"
#include <exception>

class VertexShader
{
public:
	VertexShader(const void* shaderByteCode, size_t byteCodeSize, RenderSystem* system);
	~VertexShader();
private:
	RenderSystem* m_system = nullptr;
	ID3D11VertexShader* m_vs;

	friend class DeviceContext;
	friend class RenderSystem;
};