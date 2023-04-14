#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
}

bool IndexBuffer::load(void* listIndicies, UINT sizeList)
{
	if (m_buffer) m_buffer->Release();

	D3D11_BUFFER_DESC buffDesc = {};
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.ByteWidth = sizeList * sizeof(UINT);
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = listIndicies;

	m_sizeList = sizeList;

	HRESULT result = GraphicsEngine::get()->m_d3dDevice->CreateBuffer(&buffDesc, &initData, &m_buffer);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

UINT IndexBuffer::getSizeIndexList()
{
	return this->m_sizeList;
}

bool IndexBuffer::release()
{
	m_buffer->Release();
	delete this;

	return true;
}
