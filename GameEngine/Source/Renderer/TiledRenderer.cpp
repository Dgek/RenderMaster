#include "Core.h"

#include "TiledRenderer.h"

#include "Cameras/Camera.h"
#include "Lights/Light.h"
#include "Meshes/Mesh.h"

#include "../Graphics/Other/Viewport.h"

#include "../Graphics/States/BlendState.h"
#include "../Graphics/States/DepthStencilState.h"
#include "../Graphics/States/RasterizerState.h"
#include "../Graphics/States/SamplerState.h"

#include "../Graphics/Resources/Buffers/ConstantBuffer.h"
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

unsigned int TiledRenderer::m_iNumLights = 256;

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

	/////////////////////////////////////////////////////
	//Initialize data for light culling
	/////////////////////////////////////////////////////
	m_pLightGeometryData = make_unique<LightGeometry[]>(m_iNumLights);
	m_pLightParams = make_unique<LightParams[]>(m_iNumLights);

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
	DX11API::ClearDepthStencilView(true, false, 1.0f, 0xFF, m_pDepthDSV.get());
	m_pDepthEnableStencilDisableStandard->Bind(0xFF);
	//m_pDepthDisableStencilDisable->Set(0);
	//SetDepthStencilView(m_pDepthDSV);
	//AllEnabledBackCullingRasterizer()->Set();
	m_pAllDisabledBackCullingRasterizer->Bind();
	//NoCullingStandardRasterizer()->Set();

	//SetRenderTargetView();
	DX11API::BindGlobalRenderTargetView(m_pDepthDSV.get());
	m_pFinalShadingShaders->VBind();

	m_pLightsSRV->Bind(0, ST_Pixel);
	m_pLightIdxSRV->Bind(1, ST_Pixel);
	m_pLowerBoundSRV->Bind(2, ST_Pixel);
	m_pHigherBoundSRV->Bind(3, ST_Pixel);
	m_pLightCounterSRV->Bind(4, ST_Pixel);

	m_pSunShadowSRV->Bind(10, ST_Pixel);

	m_pLinearTiledSampler->Bind(0, ST_Pixel);

	m_pcb16Bytes->UpdateSubresource(0, nullptr, &m_pCurrentCamera->GetPosition(), 0, 0);
	m_pcb16Bytes->Bind(0, ST_Pixel);

	for (auto lit = begin(m_lights); lit != end(m_lights); ++lit)
	{
		Light* pLight = (*lit).get();
		Mat4x4 shadowViewProj = pLight->GetShadowViewProj();
		shadowViewProj.Transpose();

		m_pcb64Bytes->UpdateSubresource(0, nullptr, &shadowViewProj, 0, 0);
		m_pcb64Bytes->Bind(1, ST_Pixel);
	}

	//m_viewport.Bind();
	DX11API::BindGlobalViewport();
}

void TiledRenderer::UpdateLightBuffers()
{
	m_iNumLights = m_lights.size();

	ZeroMemory(m_pLightGeometryData.get(), sizeof(LightGeometry)* m_iNumLights);
	ZeroMemory(m_pLightParams.get(), sizeof(LightParams)* m_iNumLights);

	D3D11_MAPPED_SUBRESOURCE geometrySubresource;
	D3D11_MAPPED_SUBRESOURCE paramsSubresource;
	ZeroMemory(&geometrySubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ZeroMemory(&paramsSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	unsigned int index = 0;
	for (auto lit = begin(m_lights); lit != end(m_lights); ++lit)
	{
		Light* pLight = (*lit).get();
		Vec pos = pLight->GetPos() * pLight->GetWorldTransform();
		m_pLightGeometryData[index].m_posAndRange = pos;
		m_pLightGeometryData[index].m_posAndRange.w = pLight->GetRange();

		m_pLightParams[index].m_color = pLight->GetColor();
		m_pLightParams[index].m_dirAndRange = pLight->GetDir();
		m_pLightParams[index].m_dirAndRange.w = pLight->GetRange();
		m_pLightParams[index].m_pos = pos;
		m_pLightParams[index].m_spotAngles = Vec(0.8f, 1.2f, 0.0f, 0.0f);

		index++;
	}

	DX11API::D3D11DeviceContext()->Map(m_psbLightGeometry->GetResourcePointer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &geometrySubresource);
	DX11API::D3D11DeviceContext()->Map(m_psbLights->GetResourcePointer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &paramsSubresource);

	memcpy(geometrySubresource.pData, m_pLightGeometryData.get(), sizeof(LightGeometry)*m_iNumLights);
	memcpy(paramsSubresource.pData, m_pLightParams.get(), sizeof(LightParams)*m_iNumLights);

	DX11API::D3D11DeviceContext()->Unmap(m_psbLightGeometry->GetResourcePointer(), 0);
	DX11API::D3D11DeviceContext()->Unmap(m_psbLights->GetResourcePointer(), 0);

	//m_psbLightGeometry->UpdateSubresource(0, nullptr, m_pLightGeometryData, 0, 0);
	//m_psbLights->UpdateSubresource(0, nullptr, m_pLightParams, 0, 0);
}

void TiledRenderer::VFinishPass()
{
}

void TiledRenderer::Voxelize()
{
	m_voxelizationShaders->VBind();
	this->m_pDepthDisableStencilDisable->Bind(0);
	m_pNoCullingStandardRasterizer->Bind();
	m_pPointClampSampler->Bind(0, ST_Pixel);

	struct ThreeMatrices
	{
		Mat4x4 orthos[3];
	} buffer;

	Mat4x4 world;

	float orthosize = 3200.0f;
	buffer.orthos[0] = Mat4x4::CreateViewMatrixLH(Vec(-5000, 500, 0, 1.0f), Vec(1.0f, 0.0f, 0.0f, 0.0f), Vec(0.0f, 1.0f, 0.0f, 0.0f)) *
		Mat4x4::CreateOrthoProjectionLH(orthosize, orthosize, 0.1f, 10000.0f);
	buffer.orthos[0].Transpose();

	buffer.orthos[1] = Mat4x4::CreateViewMatrixLH(Vec(0, -5000, 0, 1.0f), Vec(0.0f, 1.0f, 0.0f, 0.0f), Vec(0.0f, 0.0f, -1.0f, 0.0f)) *
		Mat4x4::CreateOrthoProjectionLH(orthosize, orthosize, 0.1f, 10000.0f);
	buffer.orthos[1].Transpose();

	buffer.orthos[2] = Mat4x4::CreateViewMatrixLH(Vec(0, 500, -5000, 1.0f), Vec(0.0f, 0.0f, 1.0f, 0.0f), Vec(0.0f, 1.0f, 0.0f, 0.0f)) *
		Mat4x4::CreateOrthoProjectionLH(orthosize, orthosize, 0.1f, 10000.0f);
	buffer.orthos[2].Transpose();

	m_pcb192Bytes->UpdateSubresource(0, nullptr, &buffer, 0, 0);

	m_pcb192Bytes->Bind(0, ST_Geometry);

	this->m_pNoCullingStandardRasterizer->Bind();

	m_voxelTextureRTV->BindWithUAV(1, 1, m_voxelGridUAV->GetView(), nullptr);
	m_voxelViewport.Bind();

	Mesh * pMesh;
	m_queue.Reset();

	while (pMesh = m_queue.Next().get())
	{
		//pMesh->VVoxelize(this, viewproj);
	};

	DX11API::UnbindGeometryShader();
	DX11API::UnbindUnorderedAccessViews(1, 1);
	DX11API::UnbindRenderTargetViews(1);
	DX11API::UnbindShaderResourceViews(0, 1, ST_Pixel);
}

void TiledRenderer::InjectVPLs()
{
	m_pDepthDisableStencilDisable->Bind(0);
	m_pNoCullingStandardRasterizer->Bind();
	m_dirLightGridShaders->VBind();

	m_pVoxelVB->Bind(0, 0);
	m_voxelGridSRV->Bind(0, ST_Pixel);
	m_pSunShadowSRV->Bind(1, ST_Pixel);
	//m_voxelSHRTVs->Bind(nullptr);
	m_voxelSHRTVs->Bind();

	m_gridViewport.Bind();

	for (auto lit = begin(m_lights); lit != end(m_lights); ++lit)
	{
		Light* pLight = (*lit).get();
		Mat4x4 shadowViewProj = pLight->GetShadowViewProj();
		shadowViewProj.Transpose();

		m_pcb64Bytes->UpdateSubresource(0, nullptr, &shadowViewProj, 0, 0);
		m_pcb64Bytes->Bind(1, ST_Pixel);

		DX11API::D3D11DeviceContext()->DrawInstanced(m_pVoxelVB->Count(), 64, 0, 0);
	}

	DX11API::UnbindShaderResourceViews(0, 2, ST_Pixel);
	DX11API::UnbindRenderTargetViews(3);
	DX11API::UnbindGeometryShader();
}

void TiledRenderer::PropagateVPLs(unsigned int index)
{
	if ((index % 2) == 0)
	{
		m_voxelSHSRVs->Bind(0, ST_Compute);
		m_voxelGridSRV->Bind(3, ST_Compute);
		m_voxelPropagateSHUAVs->Bind(0, ST_Compute);

		if (index == 0)
			m_propagateCS->Bind();
		else
			m_pPropagateOcclusionCS->Bind();
	}
	else
	{
		m_voxelPropagateSHSRVs->Bind(0, ST_Compute);
		m_voxelGridSRV->Bind(3, ST_Compute);
		m_voxelSHUAVs->Bind(0, ST_Compute);
		m_pPropagateOcclusionCS->Bind();
	}

	DX11API::D3D11DeviceContext()->Dispatch(8, 8, 8);

	DX11API::UnbindShaderResourceViews(0, 4, ST_Compute);
	DX11API::UnbindUnorderedAccessViews(0, 3);
}

void TiledRenderer::ApplyGlobalIllumination()
{
	m_pGlobalIllumShaders->VBind();

	//set constant buffers
	struct {
		Vec frustumRays[4];
	} rays;

	Mat4x4 rot;
	rot = rot.CreateRollPitchYaw(m_pCurrentCamera->GetRoll(), m_pCurrentCamera->GetPitch(), m_pCurrentCamera->GetYaw());

	Vec lowerleft = m_pCurrentCamera->GetFrustum().GetLowerLeftRay();
	lowerleft = lowerleft * rot;
	lowerleft = Vec::Normalize(lowerleft);

	Vec lowerright = m_pCurrentCamera->GetFrustum().GetLowerRightRay();
	lowerright = lowerright * rot;
	lowerright = Vec::Normalize(lowerright);

	Vec upperleft = m_pCurrentCamera->GetFrustum().GetUpperLeftRay();
	upperleft = upperleft * rot;
	upperleft = Vec::Normalize(upperleft);

	Vec upperright = m_pCurrentCamera->GetFrustum().GetUpperRightRay();
	upperright = upperright * rot;
	upperright = Vec::Normalize(upperright);

	rays.frustumRays[0] = lowerleft;
	rays.frustumRays[1] = lowerright;
	rays.frustumRays[2] = upperleft;
	rays.frustumRays[3] = upperright;

	m_pcb64Bytes->UpdateSubresource(0, nullptr, &rays, 0, 0);
	m_pcb64Bytes->Bind(0, ST_Geometry);

	Vec cameraPos = m_pCurrentCamera->GetPosition();
	m_pcb16Bytes->UpdateSubresource(0, nullptr, &cameraPos, 0, 0);
	m_pcb16Bytes->Bind(0, ST_Pixel);

	m_pNoCullingStandardRasterizer->Bind();
	//BlendAddStandard()->Set(0, 0);
	m_pGlobalIllumBlendState->Bind(0, 0xffffffff);

	//m_pVoxelVB->Set(0, 0);
	m_pGlobalIllumVB->Bind(0, 0);
	m_pPrepassSRVs->Bind(0, ST_Pixel);
	m_voxelPropagateSHSRVs->Bind(3, ST_Pixel);
	m_pLinearTiledSampler->Bind(0, ST_Pixel);

	DX11API::BindGlobalRenderTargetView();

	//m_viewport.Bind();
	DX11API::BindGlobalViewport();

	DX11API::D3D11DeviceContext()->Draw(m_pVoxelVB->Count(), 0);

	m_pNoBlending->Bind(nullptr, 0xffffffff);
	DX11API::UnbindShaderResourceViews(0, 6, ST_Pixel);
	DX11API::UnbindRenderTargetViews(1);
	DX11API::UnbindGeometryShader();
}

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