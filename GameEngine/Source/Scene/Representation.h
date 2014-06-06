#pragma once

#include "../Math/Vector.h"
#include "../Math/Matrix.h"
#include "../Renderer/Lights/Light.h"
#include "../Renderer/Meshes/Mesh.h"

class Scene;
class Renderer;
class EntityRepresentation
{
protected:

	typedef vector<shared_ptr<EntityRepresentation>> Representations;

	double m_lastUpdate;

	EntityRepresentationId m_id;

	//transform in parent space
	Mat4x4 m_currentWorldMatrix;
	Vec m_size;

	Representations m_children;

	// ======= Visualization ======= //
	Meshes	m_meshes;
	Lights	m_lights;

public:

	__forceinline EntityRepresentation(EntityRepresentationId id);

	__forceinline EntityRepresentation(EntityRepresentationId id, const Vec & size);

	EntityRepresentation(EntityRepresentationId id, const string & resourceMeshName);

	virtual void VAddChild(shared_ptr<EntityRepresentation> pRep);
	virtual void VRemoveChild(EntityRepresentation* pRep);

	virtual void VAddMesh(shared_ptr<Mesh> pMesh);
	virtual void VAddLight(shared_ptr<Light> pLight);

	virtual shared_ptr<EntityRepresentation> VFindChild(EntityRepresentationId id);

	//put all low-level mesh data to the renderer
	virtual void VRender(Scene * pScene, Renderer * pRenderer, double currentTime);

	__forceinline EntityRepresentationId GetId() const;

	void SetCurrentTransform(const Mat4x4 & mat, double currentTime);
	__forceinline void SetSize(float sizex, float sizey, float sizez);

protected:

	//pre-rendering modifications
	virtual void VPushParameters(Scene *pScene, double currentTime);

	//push data to the renderer
	virtual void VSendRenderingData(Scene * pScene, Renderer * pRenderer);

	//undo any changes made to the scene
	virtual void VPullParameters(Scene * pScene);
};

__forceinline EntityRepresentation::EntityRepresentation(EntityRepresentationId id)
	: m_size{ 0.0f, 0.0f, 0.0f, 0.0f }
{}

__forceinline EntityRepresentation::EntityRepresentation(EntityRepresentationId id, const Vec & size)
	: m_size{ 0.0f, 0.0f, 0.0f, 0.0f }, m_id(id), m_lastUpdate{ 0.0 }
{}

__forceinline EntityRepresentationId EntityRepresentation::GetId() const
{
	return m_id;
}

__forceinline void EntityRepresentation::SetSize(float sizex, float sizey, float sizez)
{
	m_size.x = sizex;
	m_size.y = sizey;
	m_size.z = sizez;
}