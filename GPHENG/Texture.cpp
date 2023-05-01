#include "Texture.h"
#include "iostream"

Texture::Texture(const wchar_t* fullPath) : Resource(fullPath)
{
	DirectX::ScratchImage imageData;
	HRESULT res = DirectX::LoadFromWICFile(fullPath, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, imageData);

	if (SUCCEEDED(res)) 
	{
		res = DirectX::CreateTexture(
			GraphicsEngine::get()->getRenderSystem()->m_d3dDevice,
			imageData.GetImages(),
			imageData.GetImageCount(),
			imageData.GetMetadata(),
			&m_texture);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = imageData.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = imageData.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		GraphicsEngine::get()->getRenderSystem()->m_d3dDevice->CreateShaderResourceView(
			m_texture,
			&desc,
			&m_shaderResourceView);
	}
	else 
	{
		throw std::exception("Texture not loaded");
	}
}

Texture::~Texture()
{
	m_shaderResourceView->Release();
	m_texture->Release();
}