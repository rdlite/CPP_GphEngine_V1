#include "VertexBuffer.h"
#include "RenderSystem.h"
#include <iostream>
#include <exception>

VertexBuffer::VertexBuffer(
	void* listVertices, UINT sizeVertex, UINT sizeList, 
	void* shaderByteCode, UINT sizeByteShader, RenderSystem* system) : m_system(system)
{
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

	HRESULT result = m_system->m_d3dDevice->CreateBuffer(&buffDesc, &initData, &m_buffer);

	if (FAILED(result))
	{
		throw std::exception("VertexBuffer not created successfully");
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0,},
		{"TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		12,
		D3D11_INPUT_PER_VERTEX_DATA,
		0,}
	};

	UINT sizeLayout = ARRAYSIZE(layout);

	result = m_system->m_d3dDevice->CreateInputLayout(layout, sizeLayout, shaderByteCode, sizeByteShader, &m_layout);

	if (FAILED(result))
	{
		throw std::exception("VertexBuffer not created successfully");
	}
}

UINT VertexBuffer::getSizeVertexList()
{
	return this->m_sizeList;
}

VertexBuffer::~VertexBuffer()
{
	m_layout->Release();
	m_buffer->Release();
}