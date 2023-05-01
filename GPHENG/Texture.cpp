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

		if (FAILED(res)) throw std::exception("Texture not loaded");

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = imageData.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = imageData.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = (UINT)imageData.GetMetadata().mipLevels;

		res = GraphicsEngine::get()->getRenderSystem()->m_d3dDevice->CreateSamplerState(&samplerDesc, &m_samplerState);
		
		if (FAILED(res)) throw std::exception("Texture not loaded");

		res = GraphicsEngine::get()->getRenderSystem()->m_d3dDevice->CreateShaderResourceView(
			m_texture,
			&desc,
			&m_shaderResourceView);

		if (FAILED(res)) throw std::exception("Texture not loaded");
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