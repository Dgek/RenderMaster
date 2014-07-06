#pragma once

#include "PhysicsInterfaces.h"

class NullPhysics : public IPhysics
{
public:

	//Initialize physics
	bool VInit() override { return true; }

	//sychronize with engine
	void VSyncData(double currentTime) override {}

	//Update simulation
	void VUpdate(double deltaSeconds) override {}

	///////////////////////////////////////
	/*** Add objects to the simulation ***/
	///////////////////////////////////////
	void VAddBox(Entity* pEntity, Vec & pos, Quaternion & rot, Vec & dimensions, float r32Mass, string & physMat, bool isStatic) override {}
	void VAddPlane(Entity* pEntity, Vec & pos, Quaternion & rot, Vec & halfDimensions, float r32Mass, string & physMat, bool isStatic) override {}
	void VAddSphere(Entity* pEntity, Vec & pos, Quaternion & rot, float r32Radius, float r32Mass, string & physMat, bool isStatic) override {}
	//TODO:Add more shapes!!!

	void VAddCharacter(Entity* pEntity, Vec & pos, Quaternion & rot, const Vec & dim, float mass, float maxForce, float maxSlope) override {}

	/////////////////////////////////////
	/*** Apply actions to the bodies ***/
	/////////////////////////////////////

	void VApplyForce(EntityId entityId, float deltaTime, const Vec & dir, float newtons) override {}
	void VApplyTorque(EntityId entityId, float deltaTime, const Vec & dir, float newtons) override {}
	void VKinematicMovement(EntityId entityId, Mat4x4 & mat) override {}

	/////////////////////////////////////
	//getters
	Vec VGetVelocity(EntityId entityId) override { return Vec{ 0.0f, 0.0f, 0.0f, 0.0f }; }
	Vec VGetAngularVelocity(EntityId entityId) override { return Vec{ 0.0f, 0.0f, 0.0f, 0.0f }; }
	Mat4x4 VGetTransform(EntityId entityId) override { return Mat4x4{}; }

	/////////////////////////////////////
	//setters
	void VSetVelocity(EntityId entityId, const Vec & vel) override {}
	void VSetAngularVelocity(EntityId entityId, const Vec & angularVel) override {}
};