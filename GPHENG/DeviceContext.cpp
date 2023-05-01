#include "DeviceContext.h"
#include "iostream"

DeviceContext::DeviceContext(ID3D11DeviceContext* deviceContext, RenderSystem* system) : 
	m_deviceContext(deviceContext), m_system(system)
{
}

void DeviceContext::clearRenderTargetColor(SwapChainPtr swapChain, float r, float g, float b, float a)
{
	FLOAT clearColor[] = { r, g, b, a };
	m_deviceContext->ClearRenderTargetView(swapChain->m_rtv, clearColor);
	m_deviceContext->ClearDepthStencilView(swapChain->m_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_deviceContext->OMSetRenderTargets(1, &swapChain->m_rtv, swapChain->m_dsv);
}

void DeviceContext::setVertexBuffer(const VertexBufferPtr& vertexBuffer)
{
	UINT stride = vertexBuffer->m_sizeVertex;
	UINT offset = 0;

	m_deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer->m_buffer, &stride, &offset);
	m_deviceContext->IASetInputLayout(vertexBuffer->m_layout);
}

void DeviceContext::setIndexBuffer(const IndexBufferPtr& indexBuffer)
{
	m_deviceContext->IASetIndexBuffer(indexBuffer->m_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::setConstantBuffer(const VertexShaderPtr& shader, const ConstantBufferPtr& buffer)
{
	m_deviceContext->VSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setConstantBuffer(const PixelShaderPtr& shader, const ConstantBufferPtr& buffer)
{
	m_deviceContext->PSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::drawTriangleList(UINT vertexCount, UINT startVertexIndex)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->Draw(vertexCount, startVertexIndex);
}

void DeviceContext::drawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->DrawIndexed(indexCount, startIndexLocation, startVertexIndex);
}

void DeviceContext::drawTriangleStrip(UINT vertexCount, UINT startVertexIndex)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_deviceContext->Draw(vertexCount, startVertexIndex);
}

void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_deviceContext->RSSetViewports(1, &vp);
}

void DeviceContext::setVertexShader(const VertexShaderPtr& vertexShader)
{
	m_deviceContext->VSSetShader(vertexShader->m_vs, nullptr, 0);
}

void DeviceContext::setPixelShader(const PixelShaderPtr& pixelShader)
{
	m_deviceContext->PSSetShader(pixelShader->m_ps, nullptr, 0);
}

void DeviceContext::setTexture(const VertexShaderPtr& vertexShader, const TexturePtr* texture, unsigned int texNums)
{
	ID3D11ShaderResourceView* listRes[32];
	ID3D11SamplerState* listSamples[32];
	for (unsigned int i = 0; i < texNums; i++)
	{
		listRes[i] = texture[i]->m_shaderResourceView;
		listSamples[i] = texture[i]->m_samplerState;
	}
	m_deviceContext->VSSetShaderResources(0, texNums, listRes);
	m_deviceContext->VSSetSamplers(0, texNums, listSamples);
}

void DeviceContext::setTexture(const PixelShaderPtr& pixelShader, const TexturePtr* texture, unsigned int texNums)
{
	ID3D11ShaderResourceView* listRes[32];
	ID3D11SamplerState* listSamples[32];
	for (unsigned int i = 0; i < texNums; i++)
	{
		listRes[i] = texture[i]->m_shaderResourceView;
		listSamples[i] = texture[i]->m_samplerState;
	}
	m_deviceContext->PSSetShaderResources(0, texNums, listRes);
	m_deviceContext->PSSetSamplers(0, texNums, listSamples);
}

DeviceContext::~DeviceContext()
{
	m_deviceContext->Release();
}