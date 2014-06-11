#include "Core.h"

#include "TiledRenderer.h"

#include "../Graphics/States/BlendState.h"
#include "../Graphics/States/DepthStencilState.h"
#include "../Graphics/States/RasterizerState.h"
#include "../Graphics/States/SamplerState.h"

#include "../Graphics/Resources/Buffers/StructuredBuffer.h"
#include "../Graphics/Resources/Buffers/VertexBuffer.h"
#include "../Graphics/Resources/Buffers/IndexBuffer.h"

#include "../Graphics/Resources/Textures/Texture2D.h"
#include "../Graphics/Resources/Views/DepthStencilView.h"
#include "../Graphics/Resources/Views/RenderTargetView.h"
#include "../Graphics/Resources/Views/ShaderResourceView.h"
#include "../Graphics/Resources/Views/UnorderedAccessView.h"

#include "../Graphics/Stages/Shaders/ShaderBunch.h"
#include "../Graphics/Stages/Shaders/ComputeShader.h"

TiledRenderer::TiledRenderer()
	: Renderer{}, m_pCurrentCamera{ nullptr }
{
	m_pDepthNormalTexture = make_unique<Texture2D>();
	m_pAlbedoGlossTexture = make_unique<Texture2D>();
}

bool TiledRenderer::VInitialize(HWND hWnd, unsigned int width, unsigned int height)
{
	if (!Renderer::VInitialize(hWnd, width, height))
	{
		assert(0 && "Some problem initializing Renderer occured");
	}

	////////////////////////////////////////////////////
	//Initialize data for z prepass
	////////////////////////////////////////////////////
	Texture2DParams texParams;
	texParams.Init(SCREEN_WIDTH, SCREEN_HEIGHT, 1, DXGI_FORMAT_R8G8B8A8_UNORM, true, false, true, false, 1, 0,
		1, true, false, false);
	m_pDepthNormalTexture->Create(texParams);
	m_pAlbedoGlossTexture->Create(texParams);

	ShaderResourceViewParams srvParams;
	srvParams.InitForTexture2D(texParams.Format, 1, 0, false);

	RenderTargetViewParams rtvParams;
	rtvParams.InitForTexture2D(texParams.Format, 0, true);

	return true;
}

void TiledRenderer::VPushMesh(shared_ptr<Mesh> pMesh)
{
	m_queue.VAdd(pMesh);
}

void TiledRenderer::VPushLight(shared_ptr<Light> pLight)
{
	m_lights.push_back(pLight);
}

void TiledRenderer::VUpdate(unsigned int deltaMilliseconds)
{
	m_queue.VSort();
}

void TiledRenderer::VRender()
{
	/*** Render Scene For Each Camera ***/
	for (auto iter = begin(m_cameras); iter != end(m_cameras); ++iter)
	{
		m_pCurrentCamera = (*iter).get();

		// ===================================================== //
		//						Z Prepass						 //
		// ===================================================== //
		PrepareForZPrepass();

		Mesh * pMesh;
		m_queue.Reset();
		while (pMesh = m_queue.Next().get())
		{
			RenderZPrepass(pMesh);
		};

		FinishZPrepass();

		//VGenerateShadowMaps();

		///////////////////////////////////////////////////////////
		//Voxelization
		//if (g_pEngine->GameTimeInSeconds())
		//{
		Voxelize();
		//}
		//RenderGrid(viewProj);

		///////////////////////////////////////////////////////////
		//Inject light sources to the voxel grid
		InjectVPLs();

		///////////////////////////////////////////////////////////
		//Propogate virtual point lights
		for (int i = 0; i < 9; i++)
			PropagateVPLs(i);

		// ===================================================== //
		//						Light Culling					 //
		// ===================================================== //
		UpdateLightBuffers();
		LightCulling();

		// ===================================================== //
		//						Final Shading Pass				 //
		// ===================================================== //
		//VRenderSky(pCamera, viewProj);

		PrepareForShadingPass();
		m_queue.Reset();
		while (pMesh = m_queue.Next().get())
		{
			//set states needed for rendering current mesh
			VPreRenderMesh(pMesh);

			//finally render it
			VRenderMesh(pMesh);

			//remove previous states
			VPostRenderMesh(pMesh);
		};

		FinishShadingPass();

		// ===================================================	//
		//			Global Illumination Post Process			//
		// ===================================================	//
		ApplyGlobalIllumination();

		//clear meshes queue
		m_queue.Clear();

		//clean lights
		while (!m_lights.empty())
		{
			m_lights.pop_back();
		}

		m_queue.Clear();
		VFinishPass();
	}
}

void TiledRenderer::PrepareForZPrepass()
{
	m_pDepthEnableStencilDisableStandard->Bind(0xFF);
	m_pAllDisabledBackCullingRasterizer->Bind();

	m_pPrepassRTVs->Bind();
}

void TiledRenderer::RenderZPrepass(Mesh* pMesh)
{}

void TiledRenderer::FinishZPrepass()
{}

void TiledRenderer::PrepareForShadingPass()
{
}

void TiledRenderer::UpdateLightBuffers()
{
	m_iNumLights = m_lights.size();

	ZeroMemory(m_pLightGeometryData, sizeof(LightGeometry)* m_iNumLights);
	ZeroMemory(m_pLightParams, sizeof(LightParams)* m_iNumLights);

	D3D11_MAPPED_SUBRESOURCE geometrySubresource;
	D3D11_MAPPED_SUBRESOURCE paramsSubresource;
	ZeroMemory(&geometrySubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ZeroMemory(&paramsSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	unsigned int index = 0;
	for (auto lit = begin(m_lights); lit != end(m_lights); +lit)
	{
		Light* pLight = (*lit).get();
		Vec pos = pLight->GetData()->m_pos * pLight->GetWorldTransform();
		m_pLightGeometryData[index].posANDrang = pos;
		m_pLightGeometryData[index].posANDrang.w = pLight->GetRange();

		m_pLightParams[index].color = pLight->GetData()->m_color;
		m_pLightParams[index].dirANDrange = pLight->GetData()->m_dir;
		m_pLightParams[index].dirANDrange.w = pLight->GetRange();
		m_pLightParams[index].pos = pos;
		m_pLightParams[index].spotAngles = Vector(0.8, 1.2, 0.0f, 0.0f);

		index++;
	}

	DX11API::D3D11DeviceContext()->Map(m_psbLightGeometry->GetResourcePointer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &geometrySubresource);
	DX11API::D3D11DeviceContext()->Map(m_psbLights->GetResourcePointer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &paramsSubresource);

	memcpy(geometrySubresource.pData, m_pLightGeometryData, sizeof(LightGeometry)*m_iNumLights);
	memcpy(paramsSubresource.pData, m_pLightParams, sizeof(LightParams)*m_iNumLights);

	DX11API::D3D11DeviceContext()->Unmap(m_psbLightGeometry->GetResourcePointer(), 0);
	DX11API::D3D11DeviceContext()->Unmap(m_psbLights->GetResourcePointer(), 0);

	//m_psbLightGeometry->UpdateSubresource(0, nullptr, m_pLightGeometryData, 0, 0);
	//m_psbLights->UpdateSubresource(0, nullptr, m_pLightParams, 0, 0);
}

void TiledRenderer::VFinishPass()
{
}

void TiledRenderer::Voxelize()
{}

void TiledRenderer::InjectVPLs()
{}

void TiledRenderer::PropagateVPLs(unsigned int index)
{}

void TiledRenderer::ApplyGlobalIllumination()
{}

void TiledRenderer::LightCulling()
{}

void TiledRenderer::FinalShading()
{}

void TiledRenderer::NullLightCounter()
{}

void TiledRenderer::FinishShadingPass()
{}

void TiledRenderer::VPreRenderMesh(Mesh*)
{}

void TiledRenderer::VRenderMesh(Mesh* pMesh)
{}

void TiledRenderer::VPostRenderMesh(Mesh* pMesh)
{}