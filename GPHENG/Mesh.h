#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh: public Resource
{
public:
	Mesh(const wchar_t* fullPath);
	~Mesh();
	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();
private:
	VertexBufferPtr m_vertexBuffer;
	IndexBufferPtr m_indexBuffer;

	friend class DeviceContext;
};