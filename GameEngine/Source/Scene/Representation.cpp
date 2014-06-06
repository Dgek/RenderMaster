#include "Core.h"

#include "Representation.h"

#include "../Renderer/Renderer.h"
#include "Scene.h"

EntityRepresentation::EntityRepresentation(EntityRepresentationId id, const string & resourceMeshName)
	: m_id(id), m_lastUpdate(0.0)
{}

void EntityRepresentation::VAddChild(shared_ptr<EntityRepresentation> pRep)
{
	m_children.push_back(pRep);
}

void EntityRepresentation::VRemoveChild(EntityRepresentation* pRep)
{}

void EntityRepresentation::VAddMesh(shared_ptr<Mesh> pMesh)
{
	m_meshes.push_back(pMesh);
}

void EntityRepresentation::VAddLight(shared_ptr<Light> pLight)
{
	m_lights.push_back(pLight);
}

shared_ptr<EntityRepresentation> EntityRepresentation::VFindChild(EntityRepresentationId id)
{
	//compare IDs and return 
	//representation if equal
	//if (m_id == id)	
	//	return this;

	//check out children
	for (auto iter = begin(m_children); iter != end(m_children); ++iter)
	{
		auto pRepr = (*iter);
		auto pResult = pRepr->VFindChild(id);

		if (pResult)
			return pResult;
	}

	//not found
	return nullptr;
}

void EntityRepresentation::VRender(Scene * pScene, Renderer * pRenderer, double currentTime)
{
	//push parameteres to the scene
	VPushParameters(pScene, currentTime);

	//send rendering data to the renderer
	VSendRenderingData(pScene, pRenderer);

	//render all children
	//	for (EntityRepresentation* pRepr : m_children)
	for (auto iter = begin(m_children); iter != end(m_children); ++iter)
	{
		auto pRepr = (*iter);
		pRepr->VRender(pScene, pRenderer, currentTime);
	}

	//undo all changes made to the scene
	VPullParameters(pScene);
}

void EntityRepresentation::SetCurrentTransform(const Mat4x4 & mat, double currentTime)
{
	m_currentWorldMatrix = mat;
	m_lastUpdate = currentTime;
}

void EntityRepresentation::VPushParameters(Scene *pScene, double currentTime)
{
	pScene->PushTransform(m_currentWorldMatrix);
}

void EntityRepresentation::VSendRenderingData(Scene * pScene, Renderer * pRenderer)
{
	//send all meshes to the renderer
	for (auto iter = begin(m_meshes); iter != end(m_meshes); ++iter)
	{
		auto pMesh = (*iter);
		pMesh->SetWorldTransform(pScene->GetTransform());
		pRenderer->VPushMesh(pMesh);
	}

	//send all lights to the renderer
	for (auto iter = begin(m_lights); iter != end(m_lights); ++iter)
	{
		auto pLight = (*iter);
		pLight->SetWorldTransform(pScene->GetTransform());
		pRenderer->VPushLight(pLight);
	}
}

void EntityRepresentation::VPullParameters(Scene * pScene)
{
	pScene->PopTransform();
}