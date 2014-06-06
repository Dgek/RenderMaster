#pragma once

#include "../Math/Matrix.h"
#include "Root.h"

class Scene
{
protected:
	MatrixStack m_transforms;

	unique_ptr<RootRepresentation> m_pRoot;

	typedef vector<shared_ptr<EntityRepresentation>> StaticObjects;
	StaticObjects m_staticObjects;

	float m_fixedTimeStep;

public:

	__forceinline Scene();

	virtual void VUpdate(double time, double elapsedTime);

	virtual void VRender(Renderer * pRenderer, double currentTime);

	//Representations management
	__forceinline shared_ptr<EntityRepresentation> GetRepresentation(EntityRepresentationId id);
	__forceinline void AddRepresentation(shared_ptr<EntityRepresentation> pRepresentation, RenderPass pass = RenderPass_Actors, EntityRepresentationId parentId = 0);

	__forceinline void AddStaticObject(shared_ptr<EntityRepresentation> pStatic);

	//update current world transform
	__forceinline void PushTransform(const Mat4x4 & mat);

	//get current tranform
	__forceinline const Mat4x4 & GetTransform() const;

	//remove last transform
	__forceinline void PopTransform();

	//	=============  //
	//	Accessors
	//	=============  //
	__forceinline double GetFixedTimeStep() const;
};

__forceinline Scene::Scene()
{
	m_pRoot = make_unique<RootRepresentation>();
	m_fixedTimeStep = 0.0333f;
}

__forceinline shared_ptr<EntityRepresentation> Scene::GetRepresentation(EntityRepresentationId id)
{
	return m_pRoot->VFindChild(id);
}

__forceinline void Scene::AddRepresentation(shared_ptr<EntityRepresentation> pRepresentation, RenderPass pass, EntityRepresentationId parentId)
{
	m_pRoot->VAddChild(pRepresentation, pass, parentId);
}

__forceinline void Scene::AddStaticObject(shared_ptr<EntityRepresentation> pStatic)
{
	m_staticObjects.push_back(pStatic);
}

__forceinline void Scene::PushTransform(const Mat4x4 & mat)
{
	m_transforms.PushTransform(mat);
}

__forceinline const Mat4x4 & Scene::GetTransform() const
{
	return m_transforms.Get();
}

__forceinline void Scene::PopTransform()
{
	m_transforms.Get();
}

__forceinline double Scene::GetFixedTimeStep() const 
{ 
	return m_fixedTimeStep; 
}