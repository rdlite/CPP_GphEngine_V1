#include "PixelShader.h"

PixelShader::PixelShader(const void* shaderByteCode, size_t byteCodeSize, RenderSystem* system) : m_system(system)
{
	HRESULT result = m_system->m_d3dDevice->CreatePixelShader(shaderByteCode, byteCodeSize, NULL, &m_ps);

	if (FAILED(result)) 
	{
		throw std::exception("PixelShader not created successfully");
	}
}

PixelShader::~PixelShader()
{
	m_ps->Release();
}