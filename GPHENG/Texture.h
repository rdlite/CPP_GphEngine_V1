#pragma once
#include "Resource.h"
#include <DirectXTex.h>
#include "GraphicsEngine.h"
#include <d3d11.h>

class Texture: public Resource
{
public:
	Texture(const wchar_t* fullPath);
	~Texture();
public:
	ID3D11Resource* m_texture = nullptr;
	ID3D11ShaderResourceView* m_shaderResourceView = nullptr;

	friend class DeviceContext;
};