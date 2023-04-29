#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(void* listIndicies, UINT sizeList, RenderSystem* system) : m_system(system)
{
	D3D11_BUFFER_DESC buffDesc = {};
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.ByteWidth = sizeList * sizeof(UINT);
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = listIndicies;

	m_sizeList = sizeList;

	HRESULT result = m_system->m_d3dDevice->CreateBuffer(&buffDesc, &initData, &m_buffer);

	if (FAILED(result))
	{
		throw std::exception("IndexBuffer not created successfully");
	}
}

UINT IndexBuffer::getSizeIndexList()
{
	return this->m_sizeList;
}

IndexBuffer::~IndexBuffer()
{
	m_buffer->Release();
}