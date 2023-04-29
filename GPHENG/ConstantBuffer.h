#pragma once
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include <d3d11.h>
#include "Prerequisites.h"

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT sizeBuffer, RenderSystem* system);
	void update(DeviceContextPtr context, void* buffer);
	~ConstantBuffer();
private:
	RenderSystem* m_system = nullptr;

	ID3D11Buffer* m_buffer;

	friend class DeviceContext;
};