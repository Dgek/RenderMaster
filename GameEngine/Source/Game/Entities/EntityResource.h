#pragma once

struct EntityComponent
{
	__forceinline virtual string VGetType() const =0 { return "Component"; }
};

///////////////////////////////////////////////////////////
//Graphics Component
///////////////////////////////////////////////////////////
struct GraphicsComponent : public EntityComponent
{
	wstring m_material;
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
	string VGetType() const override { return "Box"; }

	float m_fXHalfExtent;
	float m_fYHalfExtent;
	float m_fZHalfExtent;
};

struct GraphicsSphereComponent : public GraphicsComponent
{
	string VGetType() const override { return "Sphere"; }

	float m_fRadius;
};

struct GraphicsMeshComponent : public GraphicsComponent
{
	string VGetType() const override { return "Mesh"; }

	string m_filename;
};

struct GraphicsLightComponent : public GraphicsComponent
{
	Vec m_color;
};

struct GraphicsPointLightComponent : public GraphicsLightComponent
{
	string VGetType() const override { return "PointLight"; }

	Vec m_position;
	float m_range;
};

struct GraphicsDirectionalLightComponent : public GraphicsLightComponent
{
	string VGetType() const override { return "DirectionalLight"; }

	Vec m_direction;
};

struct GraphicsSpotLightComponent : public GraphicsLightComponent
{
	string VGetType() const override { return "SpotLight"; }

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
	string VGetType() const override{ return "Box"; }

	float m_fXHalfExtent;
	float m_fYHalfExtent;
	float m_fZHalfExtent;
};

struct PhysicsSphereComponent : public PhysicsComponent
{
	string VGetType() const override{ return "Sphere"; }

	float m_fRadius;
};

struct PhysicsMeshComponent : public PhysicsComponent
{
	string VGetType() const override{ return "Mesh"; }

	string m_filename;
};

struct PhysicsCharacterComponent : public PhysicsComponent
{
	string VGetType() const override{ return "Character"; }

	float m_height;
	float m_width;
	float m_depth;

	float m_forceInNewtons;
	float m_maxSlopeInRadians;
};

struct EntityResource
{
	Vec m_vPos;
	Vec m_vOffset;
	Vec m_vScale;
	Quaternion m_qRot;

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

private:
	void InitTransform(TiXmlElement* pComponent);
	void InitPhysicsComponent(TiXmlElement* pComponent);
	void InitGraphicsComponent(TiXmlElement* pComponent);

};