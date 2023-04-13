#pragma once
#include "GraphicsEngine.h"
#include "d3d11.h"

class DeviceContext;

class PixelShader
{
public:
	bool init(const void* shaderByteCode, size_t byteCodeSize);
	void release();
private:
	ID3D11PixelShader* m_ps;

	friend class DeviceContext;
};