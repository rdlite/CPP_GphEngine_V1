#pragma once
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include <d3d11.h>

class ConstantBuffer
{
public:
	bool load(void* buffer, UINT sizeBuffer);
	void update(DeviceContext* context, void* buffer);
	bool release();
private:
	ID3D11Buffer* m_buffer;

	friend class DeviceContext;
};