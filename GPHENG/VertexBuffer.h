#pragma once
#include "GraphicsEngine.h"
#include <d3d11.h>
#include "Prerequisites.h"
#include <exception>

class VertexBuffer
{
public:
	VertexBuffer(
		void* listVertices, UINT sizeVertex, UINT sizeList, 
		void* shaderByteCode, UINT sizeByteShader, RenderSystem* system);
	UINT getSizeVertexList();
	~VertexBuffer();
private:
	RenderSystem* m_system = nullptr;

	UINT m_sizeVertex;
	UINT m_sizeList;
	ID3D11Buffer* m_buffer;
	ID3D11InputLayout* m_layout;

	friend class DeviceContext;
};