#pragma once

#include "../Math/Vector.h"
#include "../Math/Matrix.h"

class Entity;
class Quaternion;
class IPhysics
{
public:

	//Initialize physics
	virtual bool VInit() = 0;

	//sychronize with engine
	virtual void VSyncData(double currentTime) = 0;

	//Update simulation
	virtual void VUpdate(double deltaSeconds) = 0;

	///////////////////////////////////////
	/*** Add objects to the simulation ***/
	///////////////////////////////////////
	virtual void VAddBox(Entity* pEntity, Vec & pos, Quaternion & rot, Vec & dimensions, float r32Mass, string & physMat, bool isStatic) = 0;
	virtual void VAddPlane(Entity* pEntity, Vec & pos, Quaternion & rot, Vec & halfDimensions, float r32Mass, string & physMat, bool isStatic) = 0;
	virtual void VAddSphere(Entity* pEntity, Vec & pos, Quaternion & rot, float r32Radius, float r32Mass, string & physMat, bool isStatic) = 0;
	//TODO:Add more shapes!!!

	virtual void VAddCharacter(Entity* pEntity, Vec & pos, Quaternion & rot, const Vec & dim, float mass, float maxForce, float maxSlope) = 0;

	/////////////////////////////////////
	/*** Apply actions to the bodies ***/
	/////////////////////////////////////

	virtual void VApplyForce(EntityId entityId, float deltaTime, const Vec & dir, float newtons) = 0;
	virtual void VApplyTorque(EntityId entityId, float deltaTime, const Vec & dir, float newtons) = 0;
	virtual void VKinematicMovement(EntityId entityId, Mat4x4 & mat) = 0;

	/////////////////////////////////////
	//getters
	virtual Vec VGetVelocity(EntityId entityId) = 0;
	virtual Vec VGetAngularVelocity(EntityId entityId) = 0;
	virtual Mat4x4 VGetTransform(EntityId entityId) = 0;

	/////////////////////////////////////
	//setters
	virtual void VSetVelocity(EntityId entityId, const Vec & vel) = 0;
	virtual void VSetAngularVelocity(EntityId entityId, const Vec & angularVel) = 0;
	//AVIRTUAL AVOID VSetTransform(const EntityId entityId, const Mat4x4 & mat);
};