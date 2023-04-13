#include "PixelShader.h"

bool PixelShader::init(const void* shaderByteCode, size_t byteCodeSize)
{
	HRESULT result = GraphicsEngine::get()->m_d3dDevice->CreatePixelShader(shaderByteCode, byteCodeSize, NULL, &m_ps);

	return SUCCEEDED(result);
}

void PixelShader::release()
{
	m_ps->Release();
	delete this;
}