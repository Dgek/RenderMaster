#include "Core.h"

#include "TiledRenderer.h"

TiledRenderer::TiledRenderer()
	: Renderer{}, m_pCurrentCamera{ nullptr }
{}

bool TiledRenderer::VInitialize(HWND hWnd, unsigned int width, unsigned int height)
{
	if (!Renderer::VInitialize(hWnd, width, height))
	{
		assert(0 && "Some problem initializing Renderer occured");
	}

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
	m_pDepthStencilDisableStandard->Set(0xFF);
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