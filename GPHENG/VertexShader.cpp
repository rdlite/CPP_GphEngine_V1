#include "VertexShader.h"

VertexShader::VertexShader(
	const void* shaderByteCode, size_t byteCodeSize, RenderSystem* system) : m_system(system)
{
	HRESULT result = m_system->m_d3dDevice->CreateVertexShader(shaderByteCode, byteCodeSize, NULL, &m_vs);

	if (FAILED(result))
	{
		throw std::exception("VertexBuffer not created successfully");
	}
}

VertexShader::~VertexShader()
{
	m_vs->Release();
}