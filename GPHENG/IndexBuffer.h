#pragma once
#include "GraphicsEngine.h"
#include <d3d11.h>
#include <exception>
#include "Prerequisites.h"

class IndexBuffer
{
public:
	IndexBuffer(
		void* listIndicies, UINT sizeList, RenderSystem* system);
	UINT getSizeIndexList();
	~IndexBuffer();
private:
	RenderSystem* m_system = nullptr;

	UINT m_sizeList;
	ID3D11Buffer* m_buffer;

	friend class DeviceContext;
};