struct PhysicsMaterial
{
	float m_r32Restitution;
	float m_r32Friction;

	PhysicsMaterial();

	PhysicsMaterial(float r32Restitution, float r32Friction);
};

__forceinline PhysicsMaterial::PhysicsMaterial()
	: m_r32Restitution{ 0.0f }, m_r32Friction{ 0.0f }
{
}

__forceinline PhysicsMaterial::PhysicsMaterial(float r32Restitution, float r32Friction)
	: m_r32Restitution{ r32Restitution }, m_r32Friction{ r32Friction }
{
}

class IPhysics;
unique_ptr<IPhysics> CreateHavokPhysics();