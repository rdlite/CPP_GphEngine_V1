#pragma once
#include "GraphicsEngine.h"
#include "d3d11.h"

class DeviceContext;

class VertexShader
{
public:
	bool init(const void* shaderByteCode, size_t byteCodeSize);
	void release();
private:
	ID3D11VertexShader* m_vs;

	friend class DeviceContext;
};