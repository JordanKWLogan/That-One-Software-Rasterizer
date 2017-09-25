#pragma once

#include "GraphicsDefines.h"
#include <stdint.h>


class Display
{
	public:

	void CreateDevice(void* hwnd, uint32_t swapWidth, uint32_t swapHeight);
	void CreateBuffers(uint32_t width, uint32_t height);
	void CreateShader(void);
	void CopyToBackbuffer(void);

	void Persent(void);

	void StartRender(void);
	void Clear(unsigned char R, unsigned char G, unsigned char B);
	void SetPixel(unsigned char R, unsigned char G, unsigned char B, unsigned x, unsigned y);
	void EndRender(void);

	private:

	uint32_t m_Width;
	uint32_t m_Height;

	uint32_t m_SwapWidth;
	uint32_t m_SwapHeight;

	D3D11_MAPPED_SUBRESOURCE      m_Map;

	ID3D11DevicePtr               m_Device;
	ID3D11DeviceContextPtr        m_DeviceContext;
	IDXGISwapChainPtr             m_SwapChain;
	ID3D11Texture2DPtr            m_CPUTexture;
	ID3D11Texture2DPtr            m_Texture;
	ID3D11SamplerStatePtr         m_Sampler;
	ID3D11UnorderedAccessViewPtr  m_UAVBackbuffer;
	ID3D11RenderTargetViewPtr     m_RenderTarget;
	ID3D11ShaderResourceViewPtr   m_TextureView;
	ID3D11ComputeShaderPtr        m_Shader;
};