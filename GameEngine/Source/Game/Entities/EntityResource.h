#pragma once

#include "../../Math/Vector.h"
#include "../../Math/Quaternion.h"

struct EntityComponent
{
	__forceinline virtual string VGetType() const =0 { return "Component"; }
};

///////////////////////////////////////////////////////////
//Graphics Component
///////////////////////////////////////////////////////////
struct GraphicsComponent : public EntityComponent
{
	string m_material;
};

struct GraphicsPlaneComponent : public GraphicsComponent
{
	virtual string VGetType() const override { return "Plane"; }

	float m_fXHalfExtent;
	float m_fYHalfExtent;
	float m_fZHalfExtent;
};

struct GraphicsSpriteComponent : public GraphicsComponent
{
	virtual string VGetType() const override { return "Sprite"; }

	float m_fXHalfExtent;
	float m_fYHalfExtent;
};

struct GraphicsBoxComponent : public GraphicsComponent
{
	virtual string VGetType() const override { return "Box"; }

	float m_fXHalfExtent;
	float m_fYHalfExtent;
	float m_fZHalfExtent;
};

struct GraphicsSphereComponent : public GraphicsComponent
{
	virtual string VGetType() const override { return "Sphere"; }

	float m_fRadius;
};

struct GraphicsMeshComponent : public GraphicsComponent
{
	virtual string VGetType() const override { return "Mesh"; }

	string m_filename;
};

struct GraphicsLightComponent : public GraphicsComponent
{
	Vec m_color;
};

struct GraphicsPointLightComponent : public GraphicsLightComponent
{
	virtual string VGetType() const override { return "PointLight"; }

	Vec m_position;
	float m_range;
};

struct GraphicsDirectionalLightComponent : public GraphicsLightComponent
{
	virtual string VGetType() const override { return "DirectionalLight"; }

	Vec m_direction;
};

struct GraphicsSpotLightComponent : public GraphicsLightComponent
{
	virtual string VGetType() const override { return "SpotLight"; }

	Vec m_direction;
	Vec m_position;

	float m_innerAngle;
	float m_outerAngle;
	float m_range;
};


///////////////////////////////////////////////////////////
//Physics Component
///////////////////////////////////////////////////////////
struct PhysicsComponent : public EntityComponent
{
	//Convert to actual values when creating physics model
	string m_material;
	string	m_density;
	float	m_mass;

	bool	m_bIsStatic;
};

struct PhysicsPlaneComponent : public PhysicsComponent
{
	virtual string VGetType() const override { return "Plane"; }

	float m_fXHalfExtent;
	float m_fYHalfExtent;
	float m_fZHalfExtent;
};

struct PhysicsBoxComponent : public PhysicsComponent
{
	virtual string VGetType() const override{ return "Box"; }

	float m_fXHalfExtent;
	float m_fYHalfExtent;
	float m_fZHalfExtent;
};

struct PhysicsSphereComponent : public PhysicsComponent
{
	virtual string VGetType() const override{ return "Sphere"; }

	float m_fRadius;
};

struct PhysicsMeshComponent : public PhysicsComponent
{
	virtual string VGetType() const override{ return "Mesh"; }

	string m_filename;
};

struct PhysicsCharacterComponent : public PhysicsComponent
{
	virtual string VGetType() const override{ return "Character"; }

	float m_height;
	float m_width;
	float m_depth;

	float m_forceInNewtons;
	float m_maxSlopeInRadians;
};

class IPhysics;
class TiXmlElement;
class Entity;
class Game;
class Scene;
struct EntityResource
{
	Vec m_vPos;
	Vec m_vOffset;
	Vec m_vScale;
	Quaternion m_qRot;

	Vec m_changedColor;
	bool m_bChangedColor;

	////////////////////////////
	//Entity Components
	typedef vector<shared_ptr<GraphicsComponent>> Graphics;
	typedef vector<shared_ptr<PhysicsComponent>> Physics;

	Graphics m_graphics;
	Physics  m_physics;

	explicit EntityResource(const string & filename);

	//	 =====================	 //
	//		Factory Pattern		 //
	//	 =====================	 //

	//create entity
	virtual shared_ptr<Entity> VCreateEntity(Game * pGame);

	//create its representation
	virtual void VCreateRepresentation(Scene * pScene, shared_ptr<Entity> pEntity);

	//create its physics model
	virtual void VCreatePhysicalBody(IPhysics * pPhysics, shared_ptr<Entity> pEntity);

	__forceinline void ChangeColor(float red, float green, float blue);

private:
	void InitTransform(TiXmlElement* pComponent);
	void InitPhysicsComponent(TiXmlElement* pComponent);
	void InitGraphicsComponent(TiXmlElement* pComponent);
};

__forceinline void EntityResource::ChangeColor(float red, float green, float blue)
{
	m_changedColor = Vec{ red, green, blue, 1.0f };
	m_bChangedColor = true;
}