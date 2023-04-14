#pragma once
#include "GraphicsEngine.h"
#include <d3d11.h>

class IndexBuffer
{
public:
	IndexBuffer();
	bool load(void* listIndicies, UINT sizeList);
	UINT getSizeIndexList();
	bool release();
private:
	UINT m_sizeList;
	ID3D11Buffer* m_buffer;

	friend class DeviceContext;
};