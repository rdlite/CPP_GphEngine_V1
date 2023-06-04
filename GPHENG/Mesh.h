#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <vector>

struct MaterialSlot 
{
	size_t startIndex = 0;
	size_t numIndicies = 0;
	size_t materialID = 0;
};

class Mesh: public Resource
{
public:
	Mesh(const wchar_t* fullPath);
	~Mesh();
	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();
	const MaterialSlot& getMaterialSlot(unsigned int slot);
	size_t getNumMaterialSlots();
private:
	VertexBufferPtr m_vertexBuffer;
	IndexBufferPtr m_indexBuffer;
	std::vector<MaterialSlot> m_materialSlots;

	friend class DeviceContext;
};