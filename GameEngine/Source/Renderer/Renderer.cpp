#include "Core.h"

#include "Renderer.h"

#include "../Graphics/General.h"
#include "../Graphics/Resources/Buffers/BufferParams.h"
#include "../Graphics/Resources/Buffers/ConstantBuffer.h"
#include "../Graphics/States/SamplerState.h"

#include "../Graphics/Resources/Textures/Texture2D.h"
#include "../Graphics/Resources/Views/DepthStencilView.h"

unsigned int Renderer::m_numSamples = 8;
unsigned int Renderer::m_sampleQuality = 1;
bool Renderer::m_bMSAA = true;

Renderer::Renderer()
{
	m_pDepthTexture = make_unique<Texture2D>();
	m_pDepthDSV = make_unique<DepthStencilView>();

	m_pcb16Bytes = make_unique<ConstantBuffer>();
	m_pcb64Bytes = make_unique<ConstantBuffer>();
	m_pcb128Bytes = make_unique<ConstantBuffer>();
	m_pcb192Bytes = make_unique<ConstantBuffer>();
	m_pcb256Bytes = make_unique<ConstantBuffer>();
}

Renderer::~Renderer()
{}

bool Renderer::VInitialize(HWND hWnd, unsigned int width, unsigned int height)
{
	DX11API::InitializeGraphics(hWnd);

	/*	===============================
	==	Create Depth Textures  ==
	===============================  */
	Texture2DParams texParams;
	texParams.Init(SCREEN_WIDTH, SCREEN_HEIGHT, 1, DXGI_FORMAT_D24_UNORM_S8_UINT,
		false, false, false, true, m_numSamples, m_sampleQuality, 1, true, false, false);
	if (!m_pDepthTexture->Create(texParams))
		return false;

	DepthStencilViewParams dsvParams;
	dsvParams.InitForTexture2D(DXGI_FORMAT_D24_UNORM_S8_UINT, 0, m_bMSAA);
	if (!m_pDepthTexture->CreateDepthStencilView(&m_pDepthDSV->m_pView, dsvParams))
		return false;

	/*	===============================
	==	Create Constant Buffers  ==
	===============================  */

	BufferParams params{};
	params.FillConstantBufferParams(16, true, false, false);
	if (!m_pcb16Bytes->Create(params, nullptr)) return false;

	params.FillConstantBufferParams(64, true, false, false);
	if (!m_pcb64Bytes->Create(params, nullptr)) return false;

	params.FillConstantBufferParams(128, true, false, false);
	if (!m_pcb128Bytes->Create(params, nullptr)) return false;

	params.FillConstantBufferParams(192, true, false, false);
	if (!m_pcb192Bytes->Create(params, nullptr)) return false;

	params.FillConstantBufferParams(256, true, false, false);
	if (!m_pcb256Bytes->Create(params, nullptr)) return false;
	
	return true;
}

void Renderer::VAddCamera(shared_ptr<Camera> pCamera)
{
	m_cameras.push_back(pCamera);
}

void Renderer::VRemoveCamera(Camera* pCamera)
{
	auto camera = begin(m_cameras);
	for (; camera != end(m_cameras); ++camera)
		if ((*camera).get() == pCamera)
			break;

	if (camera != end(m_cameras))
		m_cameras.erase(camera);

	assert(0 && "No such camera was found!");
}

SamplerState* Renderer::LinearTiledSampler()
{
	if (m_pLinearTiledSampler)
		return m_pLinearTiledSampler.get();

	//it's not created yet
	m_pLinearTiledSampler = make_unique<SamplerState>();

	SamplerStateParams params;
	params.Init(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, 0, 16, D3D11_COMPARISON_NEVER);
	m_pLinearTiledSampler->Create(&params);

	return m_pLinearTiledSampler.get();
}

SamplerState* Renderer::AnisotropySampler16()
{
	if (m_pAnisotropySampler16)	
		return m_pAnisotropySampler16.get();

	//it's not created yet
	m_pAnisotropySampler16 = make_unique<SamplerState>();

	SamplerStateParams params;
	params.Init(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, 0, 16, D3D11_COMPARISON_NEVER);
	m_pAnisotropySampler16->Create(&params);

	return m_pAnisotropySampler16.get();
}

SamplerState* Renderer::PointClampSampler()
{
	if (m_pPointClampSampler)
		return m_pPointClampSampler.get();

	//notcreated yet
	m_pPointClampSampler = make_unique<SamplerState>();

	SamplerStateParams params;
	params.Init(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP,
		0, 16, D3D11_COMPARISON_NEVER);
	m_pPointClampSampler->Create(&params);

	return m_pPointClampSampler.get();
}

SamplerState* Renderer::LinearLessEqualSampler()
{
	if (m_pLinearLessEqualSampler)
		return m_pLinearLessEqualSampler.get();

	//not created yet
	m_pLinearLessEqualSampler = make_unique<SamplerState>();

	SamplerStateParams params;
	params.Init(D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_MIRROR, D3D11_TEXTURE_ADDRESS_MIRROR, D3D11_TEXTURE_ADDRESS_MIRROR,
		0, 16, D3D11_COMPARISON_LESS_EQUAL);
	m_pLinearLessEqualSampler->Create(&params);

	return m_pLinearLessEqualSampler.get();
}