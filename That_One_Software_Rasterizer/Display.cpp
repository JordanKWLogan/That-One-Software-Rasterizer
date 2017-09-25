

#include "Display.h"

#include <string>

#include <D3DCompiler.h>

Display* g_NotGreatDisplay;

void Display::CreateDevice(void* hwnd, uint32_t swapWidth, uint32_t swapHeight)
{
	m_SwapWidth = swapWidth;
	m_SwapHeight = swapHeight;

	g_NotGreatDisplay = this;

	UINT createDeviceFlags = 0;

#if (defined(DEBUG) || defined(_DEBUG))
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featrueLevelArray[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT result = D3D11CreateDevice(
		0,                          // default device
		D3D_DRIVER_TYPE_HARDWARE,
		0,                          // no software device
		createDeviceFlags,
		featrueLevelArray,
		sizeof(featrueLevelArray) / sizeof(featrueLevelArray[0]),
		D3D11_SDK_VERSION,
		&m_Device,
		&featureLevel,
		&m_DeviceContext);

	if(FAILED(result))
		return;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."
	IDXGIDevice * pDXGIDevice;
	HRESULT hr = m_Device->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
	//ERRORIF(hr != S_OK, "Failed To get IDXGIDevice");

	IDXGIAdapter * pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);
	//ERRORIF(hr != S_OK, "Failed To get IDXGIAdapter");

	IDXGIFactory * pIDXGIFactory;
	hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);
	//ERRORIF(hr != S_OK, "Failed To get IDXGIFactory");

	// Create SwapChain Desc
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;

	SwapChainDesc.Windowed = true;
	SwapChainDesc.BufferDesc.Width = m_SwapWidth;
	SwapChainDesc.BufferDesc.Height = m_SwapHeight;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;


	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;


	// no MSAA, TODO: add MSAA
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;

	SwapChainDesc.BufferUsage = DXGI_USAGE_UNORDERED_ACCESS;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = 0;
	SwapChainDesc.OutputWindow = (HWND)hwnd;



	// create the swap chain
	hr = pIDXGIFactory->CreateSwapChain(
		m_Device.Get(),
		&SwapChainDesc,
		&m_SwapChain);
	//ERRORIF(hr != S_OK, "Failed To create SwapChain");


	// no automatic alt enter
	pIDXGIFactory->MakeWindowAssociation((HWND)hwnd, DXGI_MWA_NO_ALT_ENTER);

	// release all pointers we got getting the swap chain
	pIDXGIFactory->Release();
	pDXGIAdapter->Release();
	pDXGIDevice->Release();

	// create the main render view
	ID3D11Texture2DPtr BackBuffer = nullptr;
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(BackBuffer.GetAddressOf()));
	//ERRORIF(hr != S_OK, "Failed To Get Back buffer");

	//hr = m_Device->CreateRenderTargetView(BackBuffer, 0, &m_RenderTarget);
	//ERRORIF(hr != S_OK, "Failed To Create Render Target");


	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	UAVDesc.Texture2D.MipSlice = 0;

	m_Device->CreateUnorderedAccessView(BackBuffer.Get(), &UAVDesc, &m_UAVBackbuffer);

	BackBuffer.ReleaseAndGetAddressOf();


}

void Display::CreateBuffers(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	D3D11_TEXTURE2D_DESC TextDesc;
	TextDesc.Width = m_Width;
	TextDesc.Height = m_Height;
	TextDesc.MipLevels = 1;
	TextDesc.ArraySize = 1;
	TextDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	TextDesc.SampleDesc.Count = 1;
	TextDesc.SampleDesc.Quality = 0;
	TextDesc.Usage = D3D11_USAGE_STAGING;
	TextDesc.BindFlags = 0;
	TextDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TextDesc.MiscFlags = 0;

	m_Device->CreateTexture2D(&TextDesc, 0, &m_CPUTexture);

	TextDesc.Usage = D3D11_USAGE_DEFAULT;
	TextDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextDesc.CPUAccessFlags = 0;

	m_Device->CreateTexture2D(&TextDesc, 0, &m_Texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRDesc;
	SRDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	SRDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRDesc.Texture2D.MipLevels = 1;
	SRDesc.Texture2D.MostDetailedMip = 0;

	m_Device->CreateShaderResourceView(m_Texture.Get(), &SRDesc, &m_TextureView);

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.MaxLOD = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MipLODBias = 0;
	samplerDesc.BorderColor[0] = 1;
	samplerDesc.BorderColor[1] = 1;
	samplerDesc.BorderColor[2] = 1;
	samplerDesc.BorderColor[3] = 1;

	m_Device->CreateSamplerState(&samplerDesc, &m_Sampler);
}

void Display::CreateShader(void)
{
	std::string shaderCode =
		"                                                   \
	RWTexture2D<unorm float4>     rwt_Output;               \
	Texture2D<float4>       t_Texture;                \
	sampler                 s_Sampler;                \
													  \
	[numthreads(16, 16, 1)]                           \
	void ComputeMain(                                 \
	  uint3 groupId : SV_GroupID,                     \
	  uint3 groupThreadId : SV_GroupThreadID,         \
	  uint groupIndex : SV_GroupIndex,                \
	  uint3 dispatchThreadId : SV_DispatchThreadID)   \
	{                                                 \
	  float width, height;                            \
	  rwt_Output.GetDimensions(width, height);                   \
	  float2 uv = dispatchThreadId.xy / float2(width, height);            \
	  rwt_Output[dispatchThreadId.xy] = t_Texture.SampleLevel(s_Sampler, uv, 0);  \
	}                                                                     \
  ";

	ID3DBlobPtr blob = nullptr;
	ID3DBlobPtr m_Errors = nullptr;
	HRESULT hr = D3DCompile(shaderCode.c_str(), shaderCode.size(), "",
							nullptr, 0, "ComputeMain", "cs_5_0", 0, 0, &blob, &m_Errors);
	if(FAILED(hr))
	{
		std::string s((char*)m_Errors->GetBufferPointer(), m_Errors->GetBufferSize());
		return;
	}


	m_Device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_Shader);

	m_DeviceContext->CSSetShader(m_Shader.Get(), 0, 0);

	m_DeviceContext->CSSetShaderResources(0, 1, m_TextureView.GetAddressOf());

	m_DeviceContext->CSSetUnorderedAccessViews(0, 1, m_UAVBackbuffer.GetAddressOf(), 0);

	m_DeviceContext->CSSetSamplers(0, 1, m_Sampler.GetAddressOf());
}

void Display::CopyToBackbuffer(void)
{
	m_DeviceContext->CopyResource(m_Texture.Get(), m_CPUTexture.Get());

	m_DeviceContext->Dispatch(int(std::ceil(m_SwapWidth / 16) + 1), int(std::ceil(m_SwapHeight / 16) + 1), 1);
}

void Display::Persent(void)
{
	m_SwapChain->Present(0, 0);
}

void Display::StartRender(void)
{
	m_DeviceContext->Map(m_CPUTexture.Get(), 0, D3D11_MAP_WRITE, 0, &m_Map);
}

void Display::Clear(unsigned char R, unsigned char G, unsigned char B)
{
	for(unsigned y = 0; y < m_Height; ++y)
	{
		for(unsigned x = 0; x < m_Width; ++x)
		{
			*(reinterpret_cast<char*>(m_Map.pData) + (y * m_Width + x) * 4 + 0) = R;
			*(reinterpret_cast<char*>(m_Map.pData) + (y * m_Width + x) * 4 + 1) = G;
			*(reinterpret_cast<char*>(m_Map.pData) + (y * m_Width + x) * 4 + 2) = B;
			*(reinterpret_cast<char*>(m_Map.pData) + (y * m_Width + x) * 4 + 3) = 0;
		}
	}
}

void Display::SetPixel(unsigned char R, unsigned char G, unsigned char B, unsigned x, unsigned y)
{
	*(reinterpret_cast<char*>(m_Map.pData) + (y * m_Width + x) * 4 + 0) = R;
	*(reinterpret_cast<char*>(m_Map.pData) + (y * m_Width + x) * 4 + 1) = G;
	*(reinterpret_cast<char*>(m_Map.pData) + (y * m_Width + x) * 4 + 2) = B;
	*(reinterpret_cast<char*>(m_Map.pData) + (y * m_Width + x) * 4 + 3) = 0;
}

void Display::EndRender(void)
{
	m_DeviceContext->Unmap(m_CPUTexture.Get(), 0);
	m_Map.pData = 0;
}
