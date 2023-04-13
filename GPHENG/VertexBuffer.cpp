#include "VertexBuffer.h"

VertexBuffer::VertexBuffer() : m_layout(0), m_buffer(0)
{

}

bool VertexBuffer::load(void* listVertices, UINT sizeVertex, UINT sizeList, void* shaderByteCode, UINT sizeByteShader)
{
	if (m_buffer) m_buffer->Release();
	if (m_layout) m_layout->Release();

	D3D11_BUFFER_DESC buffDesc = {};
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.ByteWidth = sizeVertex * sizeList;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = listVertices;

	m_sizeVertex = sizeVertex;
	m_sizeList = sizeList;

	HRESULT result = GraphicsEngine::get()->m_d3dDevice->CreateBuffer(&buffDesc, &initData, &m_buffer);

	if (FAILED(result)) 
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0,},
		{"COLOR", 0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		12,
		D3D11_INPUT_PER_VERTEX_DATA,
		0,}
	};

	UINT sizeLayout = ARRAYSIZE(layout);

	result = GraphicsEngine::get()->m_d3dDevice->CreateInputLayout(layout, sizeLayout, shaderByteCode, sizeByteShader, &m_layout);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

UINT VertexBuffer::getSizeVertexList()
{
	return this->m_sizeList;
}

bool VertexBuffer::release()
{
	m_layout->Release();
	m_buffer->Release();
	delete this;

	return true;
}
