#include "ConstantBuffer.h"

bool ConstantBuffer::load(void* buffer, UINT sizeBuffer)
{
	if (m_buffer) m_buffer->Release();

	D3D11_BUFFER_DESC buffDesc = {};
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.ByteWidth = sizeBuffer;
	buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = buffer;

	HRESULT result = GraphicsEngine::get()->m_d3dDevice->CreateBuffer(&buffDesc, &initData, &m_buffer);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void ConstantBuffer::update(DeviceContext* context, void* buffer)
{
	context->m_deviceContext->UpdateSubresource(this->m_buffer, NULL, NULL, buffer, NULL, NULL);
}

bool ConstantBuffer::release()
{
	m_buffer->Release();
	delete this;
	return false;
}