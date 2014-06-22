#pragma once

#include "Texture.h"

struct ShaderResourceViewParams;
struct UnorderedAccessViewParams;
struct DepthStencilViewParams;
struct RenderTargetViewParams;
class ShaderResourceView;
class UnorderedAccessView;
class DepthStencilView;
class RenderTargetView;

struct Texture2DParams : public D3D11_TEXTURE2D_DESC
{
public:
	__forceinline Texture2DParams();

	__forceinline void Init(unsigned int width, unsigned int height, unsigned int arraySize,
		unsigned int format, bool shaderResource, bool unorderedAccess, bool renderTarget,
		bool depthStencil, unsigned int sampleCount, unsigned int sampleQuality,
		unsigned int miplevels, bool gpuWrite, bool cpuRead, bool cpuWrite,
		bool generateMipMaps = false);

	__forceinline void InitCubeTexture(unsigned int width, unsigned int height,
		unsigned int arraySize, unsigned int format, bool shaderResource, bool unorderedAccess,
		bool renderTarget, bool depthStencil, unsigned int sampleCount, unsigned int sampleQuality,
		unsigned int miplevels, bool gpuWrite, bool	cpuRead, bool cpuWrite,
		bool bGenerateMipMaps = false);
};

__forceinline Texture2DParams::Texture2DParams()
{}

__forceinline void Texture2DParams::Init(unsigned int width, unsigned int height, unsigned int arraySize,
	unsigned int format, bool shaderResource, bool unorderedAccess, bool renderTarget,
	bool depthStencil, unsigned int sampleCount, unsigned int sampleQuality,
	unsigned int miplevels, bool gpuWrite, bool cpuRead, bool cpuWrite,
	bool generateMipMaps)
{
	ZeroMemory(this, sizeof(Texture2DParams));

	Width = width;
	Height = height;
	ArraySize = arraySize;
	Format = (DXGI_FORMAT)format;
	SampleDesc.Count = sampleCount;
	SampleDesc.Quality = sampleQuality;
	MipLevels = miplevels;

	//set level of accessebility by CPU and GPU
	if (cpuRead && cpuWrite && gpuWrite)
	{
		Usage = D3D11_USAGE_STAGING;
		CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}
	else if (gpuWrite && !cpuRead && !cpuWrite)
	{
		Usage = D3D11_USAGE_DEFAULT;
	}
	else if (cpuWrite && !cpuRead && !gpuWrite)
	{
		Usage = D3D11_USAGE_DYNAMIC;
		CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		Usage = D3D11_USAGE_IMMUTABLE;
	}

	//Set resource binding types
	if (shaderResource)
		BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	if (renderTarget)
		BindFlags |= D3D11_BIND_RENDER_TARGET;
	if (unorderedAccess)
		BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	if (depthStencil)
		BindFlags |= D3D11_BIND_DEPTH_STENCIL;

	if (generateMipMaps)
		MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
}

__forceinline void Texture2DParams::InitCubeTexture(unsigned int width, unsigned int height,
	unsigned int arraySize, unsigned int format, bool shaderResource, bool unorderedAccess,
	bool renderTarget, bool depthStencil, unsigned int sampleCount, unsigned int sampleQuality,
	unsigned int miplevels, bool gpuWrite, bool	cpuRead, bool cpuWrite,
	bool bGenerateMipMaps)
{
	Init(width, height, arraySize, format, shaderResource, unorderedAccess, renderTarget, depthStencil, sampleCount, sampleQuality,
		miplevels, gpuWrite, cpuRead, cpuWrite, bGenerateMipMaps);

	MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
}

class Texture2D : public ITexture
{
public:
	//Constructor
	__forceinline Texture2D();

	virtual bool Create(const D3D11_TEXTURE2D_DESC & pParams);

	void CreateFromFile(const char * filepath);

	virtual bool CreateShaderResourceView(ShaderResourceView & view,
		const ShaderResourceViewParams & params)	const;

	virtual bool CreateShaderResourceView(ID3D11ShaderResourceView** ppView,
		const ShaderResourceViewParams & params)	const;

	virtual bool CreateShaderResourceView(ID3D11ShaderResourceView** ppView) const;


	virtual bool CreateUnorderedAccessView(UnorderedAccessView & view,
		const UnorderedAccessViewParams & params)	const;

	virtual bool CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
		const UnorderedAccessViewParams & params)	const;


	virtual bool CreateRenderTargetView(RenderTargetView & view,
		const RenderTargetViewParams & params)		const;

	virtual bool CreateRenderTargetView(ID3D11RenderTargetView** ppView,
		const RenderTargetViewParams & params)		const;


	virtual bool CreateDepthStencilView(DepthStencilView & view,
		const DepthStencilViewParams & params)		const;

	virtual bool CreateDepthStencilView(ID3D11DepthStencilView* pView,
		const DepthStencilViewParams & params)		const;

	virtual bool CreateDepthStencilView(ID3D11DepthStencilView** pView,
		const DepthStencilViewParams & params)		const;


protected:
	ID3D11Texture2D* m_pTexture;
};

__forceinline Texture2D::Texture2D()
	: m_pTexture{ nullptr }
{}