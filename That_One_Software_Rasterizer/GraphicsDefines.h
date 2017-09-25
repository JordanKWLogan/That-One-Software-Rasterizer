#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX

#include <d3d11.h>
#include <DXGI1_2.h>

#include <wrl.h>

using Microsoft::WRL::ComPtr;

// common smart pointers
using ID3D11DevicePtr				= ComPtr<ID3D11Device>;
using ID3D11DeviceContextPtr		= ComPtr<ID3D11DeviceContext>;
using ID3D11RenderTargetViewPtr		= ComPtr<ID3D11RenderTargetView>;
using IDXGISwapChain1Ptr			= ComPtr<IDXGISwapChain1>;
using IDXGISwapChainPtr				= ComPtr<IDXGISwapChain>;
using IDXGISurfacePtr				= ComPtr<IDXGISurface>;
using ID3D11Texture1DPtr			= ComPtr<ID3D11Texture1D>;
using ID3D11Texture2DPtr			= ComPtr<ID3D11Texture2D>;
using ID3D11Texture3DPtr			= ComPtr<ID3D11Texture3D>;
using ID3D11DepthStencilViewPtr		= ComPtr<ID3D11DepthStencilView>;
using ID3D11UnorderedAccessViewPtr	= ComPtr<ID3D11UnorderedAccessView>;
using ID3D11InputLayoutPtr			= ComPtr<ID3D11InputLayout>;
using ID3D11RasterizerStatePtr		= ComPtr<ID3D11RasterizerState>;
using ID3D11BufferPtr				= ComPtr<ID3D11Buffer>;
using ID3DBlobPtr					= ComPtr<ID3DBlob>;
using ID3D11DeviceChildPtr			= ComPtr<ID3D11DeviceChild>;
using ID3D11SamplerStatePtr			= ComPtr<ID3D11SamplerState>;
using ID3D11ShaderResourceViewPtr	= ComPtr<ID3D11ShaderResourceView>;
using ID3D11BlendStatePtr			= ComPtr<ID3D11BlendState>;
using ID3D11DepthStencilStatePtr	= ComPtr<ID3D11DepthStencilState>;
using ID3D11ComputeShaderPtr		= ComPtr<ID3D11ComputeShader>;