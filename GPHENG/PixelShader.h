#pragma once
#include "GraphicsEngine.h"
#include "d3d11.h"
#include <exception>
#include "Prerequisites.h"

class PixelShader
{
public:
	PixelShader(const void* shaderByteCode, size_t byteCodeSize, RenderSystem* system);
	~PixelShader();
private:
	RenderSystem* m_system = nullptr;

	ID3D11PixelShader* m_ps;

	friend class DeviceContext;
	friend class RenderSystem;
};