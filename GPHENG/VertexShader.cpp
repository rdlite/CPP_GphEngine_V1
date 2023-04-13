#include "VertexShader.h"

bool VertexShader::init(const void* shaderByteCode, size_t byteCodeSize)
{
	HRESULT result = GraphicsEngine::get()->m_d3dDevice->CreateVertexShader(shaderByteCode, byteCodeSize, NULL, &m_vs);

	return SUCCEEDED(result);
}

void VertexShader::release()
{
	m_vs->Release();
	delete this;
}