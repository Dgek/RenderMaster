#pragma once

#include "Representation.h"

//enumaration for render passes
enum RenderPass : unsigned char
{
	RenderPass_Static,
	RenderPass_Actors,
	NUM_RENDER_PASSES,
};

//	=====	Root Node for the scene	=====	//
class RootRepresentation : public EntityRepresentation
{
protected:
	//devide all objects on different render passes
	Representations m_passes[NUM_RENDER_PASSES];

public:
	//constructor
	RootRepresentation();

	virtual void VAttachChild(shared_ptr<EntityRepresentation> pRep, RenderPass pass, EntityRepresentationId parentId);
	virtual void VRemoveChild(EntityRepresentation* pRep) override;

	virtual void VAddMesh(shared_ptr<Mesh> pMesh) override;
	virtual void VAddLight(shared_ptr<Light> pLight) override;

	virtual shared_ptr<EntityRepresentation> VFindChild(EntityRepresentationId id) override;
	EntityRepresentation* FindChild(EntityRepresentationId id, RenderPass pass);

	virtual void VRender(Scene * pScene, Renderer * pRenderer, double currentTime) override;

};