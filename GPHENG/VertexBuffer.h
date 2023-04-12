#pragma once
#include "GraphicsEngine.h"
#include <d3d11.h>

class DeviceContext;

class VertexBuffer
{
public:
	VertexBuffer();
	bool load(void* listVertices, UINT sizeVertex, UINT sizeList, void* shaderByteCode, UINT sizeByteShader);
	UINT getSizeVertexList();
	bool release();
private:
	UINT m_sizeVertex;
	UINT m_sizeList;
	ID3D11Buffer* m_buffer;
	ID3D11InputLayout* m_layout;

	friend class DeviceContext;
};