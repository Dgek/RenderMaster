#include "Core.h"

#include "Renderer.h"

#include "../Graphics/General.h"
#include "../Graphics/Resources/Buffers/BufferParams.h"
#include "../Graphics/Resources/Buffers/ConstantBuffer.h"
#include "../Graphics/States/BlendState.h"
#include "../Graphics/States/DepthStencilState.h"
#include "../Graphics/States/RasterizerState.h"
#include "../Graphics/States/SamplerState.h"

#include "../Graphics/Resources/Textures/Texture2D.h"
#include "../Graphics/Resources/Views/DepthStencilView.h"

Renderer::Renderer()
{
	m_pcb16Bytes = make_unique<ConstantBuffer>();
	m_pcb64Bytes = make_unique<ConstantBuffer>();
	m_pcb128Bytes = make_unique<ConstantBuffer>();
	m_pcb256Bytes = make_unique<ConstantBuffer>();
}

Renderer::~Renderer()
{}

bool Renderer::VInitialize(HWND hWnd, unsigned int width, unsigned int height)
{
	DX11API::InitializeGraphics(hWnd);

	BufferParams params{};

	/*	===============================
	==	Create Constant Buffers  ==
	===============================  */

	params.FillConstantBufferParams(16, true, false, false);
	if (!m_pcb16Bytes->Create(params, nullptr)) return false;

	params.FillConstantBufferParams(64, true, false, false);
	if (!m_pcb64Bytes->Create(params, nullptr)) return false;

	params.FillConstantBufferParams(128, true, false, false);
	if (!m_pcb128Bytes->Create(params, nullptr)) return false;

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