#include "Core.h"

#include "TiledRenderer.h"

#include "Cameras/Camera.h"
#include "Lights/Light.h"
#include "Meshes/IndexedMesh.h"

#include "../Graphics/Other/Viewport.h"

#include "../Graphics/States/BlendState.h"
#include "../Graphics/States/DepthStencilState.h"
#include "../Graphics/States/RasterizerState.h"
#include "../Graphics/States/SamplerState.h"

#include "../Graphics/Resources/Buffers/BufferParams.h"
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

unsigned int TiledRenderer::m_iNumLights = 1000;
unsigned int TiledRenderer::m_iLightsPerTile = 256;

TiledRenderer::TiledRenderer()
	: Renderer{}, m_pCurrentCamera{ nullptr }
{
	//prepass
	m_pDepthNormalTexture = make_unique<Texture2D>();
	m_pAlbedoGlossTexture = make_unique<Texture2D>();
	m_pPrepassRTVs = make_unique<RenderTargetView>(2);
	m_pPrepassSRVs = make_unique<ShaderResourceView>(2);

	m_pPrepassLayout = make_unique<INPUT_LAYOUT[]>(4);
	m_pPrepassShaders = make_unique<ShaderBunch>();

	//light culling
	m_psbLightIdx = make_unique<StructuredBuffer>();
	m_pLightIdxUAV = make_unique<UnorderedAccessView>();
	m_pLightIdxSRV = make_unique<ShaderResourceView>();

	m_psbLowerBound = make_unique<StructuredBuffer>();
	m_pLowerBoundUAV = make_unique<UnorderedAccessView>();
	m_pLowerBoundSRV = make_unique<ShaderResourceView>();

	m_psbHigherBound = make_unique<StructuredBuffer>();
	m_pHigherBoundUAV = make_unique<UnorderedAccessView>();
	m_pHigherBoundSRV = make_unique<ShaderResourceView>();

	m_psbLightCounter = make_unique<StructuredBuffer>();
	m_pLightCounterUAV = make_unique<UnorderedAccessView>();
	m_pLightCounterSRV = make_unique<ShaderResourceView>();

	m_psbLightGeometry = make_unique<StructuredBuffer>();
	m_pLightGeometrySRV = make_unique<ShaderResourceView>();

	m_psbLights = make_unique<StructuredBuffer>();
	m_pLightsSRV = make_unique<ShaderResourceView>();

	m_pcbCameraCulling = make_unique<ConstantBuffer>();

	//final shading
	m_pFinalShadingLayout = make_unique<INPUT_LAYOUT[]>(4);
	m_pFinalShadingShaders = make_unique<ShaderBunch>();

	//voxelization
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
	texParams.Init(SCREEN_WIDTH, SCREEN_HEIGHT, 1, DXGI_FORMAT_R8G8B8A8_UNORM, true, false, true, false, m_numSamples, m_sampleQuality,
		1, true, false, false);
	m_pDepthNormalTexture->Create(texParams);
	m_pAlbedoGlossTexture->Create(texParams);

	ShaderResourceViewParams srvParams;
	srvParams.InitForTexture2D(texParams.Format, 1, 0, m_bMSAA);
	m_pDepthNormalTexture->CreateShaderResourceView(m_pPrepassSRVs->GetView(0), srvParams);
	m_pAlbedoGlossTexture->CreateShaderResourceView(m_pPrepassSRVs->GetView(1), srvParams);

	RenderTargetViewParams rtvParams;
	rtvParams.InitForTexture2D(texParams.Format, 0, m_bMSAA);
	m_pDepthNormalTexture->CreateRenderTargetView(m_pPrepassRTVs->GetView(0), rtvParams);
	m_pAlbedoGlossTexture->CreateRenderTargetView(m_pPrepassRTVs->GetView(1), rtvParams);

	m_pPrepassLayout[0].SemanticName = "POSITION";
	m_pPrepassLayout[0].SemanticIndex = 0;
	m_pPrepassLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	m_pPrepassLayout[0].InputSlot = 0;
	m_pPrepassLayout[0].AlignedByteOffset = 0;
	m_pPrepassLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	m_pPrepassLayout[0].InstanceDataStepRate = 0;

	m_pPrepassLayout[1].SemanticName = "TEXCOORDS";
	m_pPrepassLayout[1].SemanticIndex = 0;
	m_pPrepassLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	m_pPrepassLayout[1].InputSlot = 1;
	m_pPrepassLayout[1].AlignedByteOffset = 0;
	m_pPrepassLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	m_pPrepassLayout[1].InstanceDataStepRate = 0;

	m_pPrepassLayout[2].SemanticName = "NORMAL";
	m_pPrepassLayout[2].SemanticIndex = 0;
	m_pPrepassLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	m_pPrepassLayout[2].InputSlot = 2;
	m_pPrepassLayout[2].AlignedByteOffset = 0;
	m_pPrepassLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	m_pPrepassLayout[2].InstanceDataStepRate = 0;

	m_pPrepassLayout[3].SemanticName = "TANGENT";
	m_pPrepassLayout[3].SemanticIndex = 0;
	m_pPrepassLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	m_pPrepassLayout[3].InputSlot = 3;
	m_pPrepassLayout[3].AlignedByteOffset = 0;
	m_pPrepassLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	m_pPrepassLayout[3].InstanceDataStepRate = 0;

	m_pPrepassShaders->SetVertexShader("shaders/prepass_vs.vso", m_pPrepassLayout.get(), 4);
	m_pPrepassShaders->SetPixelShader("shaders/prepass_ps.pso");

	/////////////////////////////////////////////////////
	//Initialize data for light culling
	/////////////////////////////////////////////////////
	m_pLightGeometryData = make_unique<LightGeometry[]>(m_iNumLights);
	m_pLightParams = make_unique<LightParams[]>(m_iNumLights);

	BufferParams bufferParams;
	bufferParams.FillStructredBufferParams(4, m_iLightsPerTile*m_iNumLights, false, true);
	m_psbLightIdx->Create(bufferParams, nullptr);

	srvParams.InitForStructuredBuffer(DXGI_FORMAT_UNKNOWN, 0, m_iLightsPerTile*m_iNumLights);
	m_psbLightIdx->CreateShaderResourceView(m_pLightIdxSRV->GetView(), srvParams);

	UnorderedAccessViewParams uavParams;
	uavParams.InitForStructuredBuffer(DXGI_FORMAT_UNKNOWN, 0, m_iLightsPerTile*m_iNumLights, D3D11_BUFFER_UAV_FLAG_COUNTER);
	m_psbLightIdx->CreateUnorderedAccessView(m_pLightIdxUAV->GetView(), uavParams);

	/*****************************************/
	//lower bound
	bufferParams.FillStructredBufferParams(4, m_iNumLights, false, true);
	m_psbLowerBound->Create(bufferParams, nullptr);

	srvParams.InitForStructuredBuffer(DXGI_FORMAT_UNKNOWN, 0, m_iNumLights);
	m_psbLowerBound->CreateShaderResourceView(m_pLowerBoundSRV->GetView(), srvParams);

	uavParams.InitForStructuredBuffer(DXGI_FORMAT_UNKNOWN, 0, m_iNumLights, D3D11_BUFFER_UAV_FLAG_COUNTER);
	m_psbLowerBound->CreateUnorderedAccessView(m_pLowerBoundUAV->GetView(), uavParams);

	/*****************************************/
	//higher bound
	bufferParams.FillStructredBufferParams(4, m_iNumLights, false, true);
	m_psbHigherBound->Create(bufferParams, nullptr);

	srvParams.InitForStructuredBuffer(DXGI_FORMAT_UNKNOWN, 0, m_iNumLights);
	m_psbHigherBound->CreateShaderResourceView(m_pHigherBoundSRV->GetView(), srvParams);

	uavParams.InitForStructuredBuffer(DXGI_FORMAT_UNKNOWN, 0, m_iNumLights, D3D11_BUFFER_UAV_FLAG_COUNTER);
	m_psbHigherBound->CreateUnorderedAccessView(m_pHigherBoundUAV->GetView(), uavParams);

	/*****************************************/
	//Light Counter
	bufferParams.FillStructredBufferParams(4, 1, false, true);
	m_psbLightCounter->Create(bufferParams, nullptr);

	srvParams.InitForStructuredBuffer(DXGI_FORMAT_UNKNOWN, 0, 1);
	m_psbLightCounter->CreateShaderResourceView(m_pLightCounterSRV->GetView(), srvParams);

	uavParams.InitForStructuredBuffer(DXGI_FORMAT_UNKNOWN, 0, 1, D3D11_BUFFER_UAV_FLAG_COUNTER);
	m_psbLightCounter->CreateUnorderedAccessView(m_pLightCounterUAV->GetView(), uavParams);

	Resource shaderResource{ "shaders/LightCulling.cso" };
	shared_ptr<ResHandle> pShaderHandle = ResourceCache::SafeGetHandle(&shaderResource);
	shared_ptr<ComputeShaderResourceExtraData> pShaderData = static_pointer_cast<ComputeShaderResourceExtraData>(pShaderHandle->GetExtra());
	m_pLightCullingShader = pShaderData->m_pShader;

	/*****************************************/
	//Light Geometry
	bufferParams.FillStructredBufferParams(sizeof(LightGeometry), m_iNumLights, true, false);
	m_psbLightGeometry->Create(bufferParams, nullptr);

	srvParams.InitForStructuredBuffer(DXGI_FORMAT_UNKNOWN, 0, m_iNumLights);
	m_psbLightGeometry->CreateShaderResourceView(m_pLightGeometrySRV->GetView(), srvParams);

	/*****************************************/
	//Light Params
	bufferParams.FillStructredBufferParams(sizeof(LightParams), m_iNumLights, true, false);
	m_psbLights->Create(bufferParams, nullptr);

	srvParams.InitForStructuredBuffer(DXGI_FORMAT_UNKNOWN, 0, m_iNumLights);
	m_psbLights->CreateShaderResourceView(m_pLightsSRV->GetView(), srvParams);

	/*****************************************/
	//Camera culling
	BufferParams params{};
	params.FillConstantBufferParams(sizeof(CameraCullingData), true, false, false);
	if (!m_pcbCameraCulling->Create(params, nullptr)) return false;

	/////////////////////////////////////////////////////
	//Initialize data for final shading
	/////////////////////////////////////////////////////
	m_pFinalShadingShaders->SetVertexShader("shaders/tiled_shading_vs.vso", m_pPrepassLayout.get(), 4);
	m_pFinalShadingShaders->SetPixelShader("shaders/tiled_shading_ps.pso");

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
		//Voxelize();
		//}
		//RenderGrid(viewProj);

		///////////////////////////////////////////////////////////
		//Inject light sources to the voxel grid
		//InjectVPLs();

		///////////////////////////////////////////////////////////
		//Propogate virtual point lights
		//for (int i = 0; i < 9; i++)
		//	PropagateVPLs(i);

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
			//render it
			VRenderMesh(pMesh);
		};

		FinishShadingPass();

		// ===================================================	//
		//			Global Illumination Post Process			//
		// ===================================================	//
		//ApplyGlobalIllumination();

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
	DepthEnableStencilDisableStandard()->Bind(0xFF);
	AllDisabledBackCullingRasterizer()->Bind();
	LinearTiledSampler()->Bind(0, ST_Pixel);

	m_pPrepassRTVs->Bind();

	m_pPrepassShaders->Bind();
}

void TiledRenderer::RenderZPrepass(Mesh* pMesh)
{
	IndexedMesh* pIndexedMesh = static_cast<IndexedMesh*>(pMesh);

	struct Buffer { Mat4x4 world; Mat4x4 worldView; Mat4x4 WVP; } buffer;
	buffer.world = pIndexedMesh->GetWorldTransform();
	buffer.worldView = buffer.world * m_pCurrentCamera->GetView();
	buffer.WVP = buffer.worldView * m_pCurrentCamera->GetProjection();
	buffer.world.Transpose(); buffer.worldView.Transpose(); buffer.WVP.Transpose();

	m_pcb192Bytes->UpdateSubresource(0, nullptr, &buffer, 0, 0);
	m_pcb192Bytes->Bind(0, ST_Vertex);

	pIndexedMesh->BindVertices(0, 0);
	pIndexedMesh->BindTexCoords(1, 0);
	pIndexedMesh->BindNormals(2, 0);
	pIndexedMesh->BindTangents(3, 0);

	pIndexedMesh->BindIndexBuffer(0);

	for (auto iter = begin(pIndexedMesh->m_subMeshes); iter != end(pIndexedMesh->m_subMeshes); ++iter)
	{
		(*iter)->BindMaterialNormal(0);
		(*iter)->BindMaterialDiffuse(1);

		(*iter)->DrawIndexed();

		(*iter)->UnBindMaterialNormal();
		(*iter)->UnBindMaterialDiffuse();
	}
}

void TiledRenderer::FinishZPrepass()
{
	DX11API::UnbindRenderTargetViews(2);
}

void TiledRenderer::PrepareForShadingPass()
{
	DX11API::ClearDepthStencilView(true, false, 1.0f, 0xFF, m_pDepthDSV.get());
	DepthEnableStencilDisableStandard()->Bind(0xFF);
	//m_pDepthDisableStencilDisable->Set(0);
	//SetDepthStencilView(m_pDepthDSV);
	//AllEnabledBackCullingRasterizer()->Set();
	AllDisabledBackCullingRasterizer()->Bind();
	//NoCullingStandardRasterizer()->Set();

	//SetRenderTargetView();
	DX11API::BindGlobalRenderTargetView(m_pDepthDSV.get());
	m_pFinalShadingShaders->Bind();

	m_pLightsSRV->Bind(0, ST_Pixel);
	m_pLightIdxSRV->Bind(1, ST_Pixel);
	m_pLowerBoundSRV->Bind(2, ST_Pixel);
	m_pHigherBoundSRV->Bind(3, ST_Pixel);
	m_pLightCounterSRV->Bind(4, ST_Pixel);

	//m_pSunShadowSRV->Bind(10, ST_Pixel);

	LinearTiledSampler()->Bind(0, ST_Pixel);

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
	m_pPrepassRTVs->Clear();
	//m_voxelTextureRTV->Clear();
	NullLightCounter();
	ClearVoxelGrid();
}

void TiledRenderer::Voxelize()
{
	m_voxelizationShaders->Bind();
	DepthDisableStencilDisable()->Bind(0);
	NoCullingStandardRasterizer()->Bind();
	PointClampSampler()->Bind(0, ST_Pixel);

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

	NoCullingStandardRasterizer()->Bind();

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
	DepthDisableStencilDisable()->Bind(0);
	NoCullingStandardRasterizer()->Bind();
	m_dirLightGridShaders->Bind();

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
	m_pGlobalIllumShaders->Bind();

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

	NoCullingStandardRasterizer()->Bind();
	//BlendAddStandard()->Set(0, 0);
	m_pGlobalIllumBlendState->Bind(0, 0xffffffff);

	//m_pVoxelVB->Set(0, 0);
	m_pGlobalIllumVB->Bind(0, 0);
	m_pPrepassSRVs->Bind(0, ST_Pixel);
	m_voxelPropagateSHSRVs->Bind(3, ST_Pixel);
	LinearTiledSampler()->Bind(0, ST_Pixel);

	DX11API::BindGlobalRenderTargetView();

	//m_viewport.Bind();
	DX11API::BindGlobalViewport();

	DX11API::D3D11DeviceContext()->Draw(m_pVoxelVB->Count(), 0);

	NoBlending()->Bind(nullptr, 0xffffffff);
	DX11API::UnbindShaderResourceViews(0, 6, ST_Pixel);
	DX11API::UnbindRenderTargetViews(1);
	DX11API::UnbindGeometryShader();
}

void TiledRenderer::ClearVoxelGrid()
{

}

void TiledRenderer::LightCulling()
{
	CameraCullingData data;
	data.View = m_pCurrentCamera->GetView(); data.View.Transpose();
	data.viewProj = m_pCurrentCamera->GetViewProjection(); data.viewProj.Transpose();
	data.posAndFov = m_pCurrentCamera->GetPosition(); data.posAndFov.w = m_pCurrentCamera->GetFrustum().GetFOV();
	data.cameraFar = VIEW_DISTANCE; data.cameraNear = 1.0f;
	data.viewWidth = SCREEN_WIDTH; data.viewHeight = SCREEN_HEIGHT; data.dir = m_pCurrentCamera->GetDir();

	m_pcbCameraCulling->UpdateSubresource(0, nullptr, &data, 0, 0);
	m_pcbCameraCulling->Bind(0, ST_Compute);

	struct culling
	{
		unsigned int m_numLights; unsigned int m_pad1;
		unsigned int m_pad2; unsigned int m_pad3;
	} culling_data;
	culling_data.m_numLights = m_iNumLights;

	m_pcb16Bytes->UpdateSubresource(0, nullptr, &culling_data, 0, 0);
	m_pcb16Bytes->Bind(1, ST_Compute);

	m_pLightCullingShader->Bind();
	//m_pLinearZSRV->Set(0, ST_Compute);
	m_pPrepassSRVs->BindView(0, 0, ST_Compute);
	m_pLightGeometrySRV->Bind(1, ST_Compute);
	m_pLightIdxUAV->Bind(0, ST_Compute);
	m_pLowerBoundUAV->Bind(1, ST_Compute);
	m_pHigherBoundUAV->Bind(2, ST_Compute);
	m_pLightCounterUAV->Bind(3, ST_Compute);

	DX11API::D3D11DeviceContext()->Dispatch(ceil((float)SCREEN_WIDTH / 16.0), ceil((float)SCREEN_HEIGHT / 16.0), 1);
	//D3D11DeviceContext()->Dispatch(1, 1, 1);

	DX11API::UnbindShaderResourceViews(0, 2, ST_Compute);
	DX11API::UnbindUnorderedAccessViews(0, 4);

	m_pLightIdxSRV->Bind(1, ST_Pixel);
}

void TiledRenderer::FinalShading()
{}

void TiledRenderer::NullLightCounter()
{
	float f = 0.0f;
	m_psbLightCounter->UpdateSubresource(0, nullptr, &f, 0, 0);
}

void TiledRenderer::FinishShadingPass()
{
	DX11API::UnbindShaderResourceViews(0, 5, ST_Pixel);
	DX11API::UnbindShaderResourceViews(10, 1, ST_Pixel);
}

void TiledRenderer::VRenderMesh(Mesh* pMesh)
{
	IndexedMesh* pIndexedMesh = static_cast<IndexedMesh*>(pMesh);

	struct Buffer { Mat4x4 world; Mat4x4 worldView; Mat4x4 WVP; } buffer;
	buffer.world = pIndexedMesh->GetWorldTransform();
	buffer.worldView = buffer.world * m_pCurrentCamera->GetView();
	buffer.WVP = buffer.worldView * m_pCurrentCamera->GetProjection();
	buffer.world.Transpose(); buffer.worldView.Transpose(); buffer.WVP.Transpose();

	m_pcb192Bytes->UpdateSubresource(0, nullptr, &buffer, 0, 0);
	m_pcb192Bytes->Bind(0, ST_Vertex);

	pIndexedMesh->BindVertices(0, 0);
	pIndexedMesh->BindTexCoords(1, 0);
	pIndexedMesh->BindNormals(2, 0);
	pIndexedMesh->BindTangents(3, 0);

	pIndexedMesh->BindIndexBuffer(0);

	for (auto iter = begin(pIndexedMesh->m_subMeshes); iter != end(pIndexedMesh->m_subMeshes); ++iter)
	{
		(*iter)->BindMaterial(5);

		(*iter)->DrawIndexed();

		(*iter)->UnbindMaterial();
	}
}