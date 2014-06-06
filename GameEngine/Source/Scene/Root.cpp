#include "Core.h"

#include "Root.h"

void RootRepresentation::VAddChild(shared_ptr<EntityRepresentation> pRep, RenderPass pass, EntityRepresentationId parentId)
{
	//if no parent for this representation than just 
	//add it to the right pass
	if (!parentId)
		m_passes[pass].push_back(pRep);
	else
	{
		if (EntityRepresentation * pParent = FindChild(parentId, pass))
		{
			pParent->VAddChild(pRep);
		}
		else
		{
			assert(0 && "Parent with such Id doesn't exists or lies in the other pass!");
		}
	}
}

void RootRepresentation::VRemoveChild(EntityRepresentation* pRep)
{}

shared_ptr<EntityRepresentation> RootRepresentation::VFindChild(EntityRepresentationId id)
{
	for (int pass = 0; pass < NUM_RENDER_PASSES; pass++)
	{
		if (auto pResult = FindChild(id, (RenderPass)pass))
		{
			//TODOreturn pResult;
		}
	}

	//not found
	return nullptr;
}

EntityRepresentation* RootRepresentation::FindChild(EntityRepresentationId id, RenderPass pass)
{
	//check out all children
	for (auto it = m_passes[pass].begin(); it != m_passes[pass].end(); it++)
	{
		auto pRepr = (*it);
		if (auto pResult = pRepr->VFindChild(id))
		{
			//TODOreturn pResult;
		}
	}

	//not found
	return nullptr;
}

void RootRepresentation::VRender(Scene * pScene, Renderer * pRenderer, double currentTime)
{
	VPushParameters(pScene, currentTime);
	VSendRenderingData(pScene, pRenderer);

	//render all passes
	for (int i = 0; i < NUM_RENDER_PASSES; i++)
	{
		//render all representations from the given pass
		for (auto it = m_passes[i].begin(); it != m_passes[i].end(); it++)
		{
			auto pRepr = (*it);
			pRepr->VRender(pScene, pRenderer, currentTime);
		}
	}

	VPullParameters(pScene);
}