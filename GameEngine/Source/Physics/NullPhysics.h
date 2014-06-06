#pragma once

#include "PhysicsInterfaces.h"

class NullPhysics : public IPhysics
{
public:

	//Initialize physics
	__forceinline bool VInit() override { return true; }

	//sychronize with engine
	__forceinline void VSyncData(double currentTime) override {}

	//Update simulation
	__forceinline void VUpdate(double deltaSeconds) override {}

	///////////////////////////////////////
	/*** Add objects to the simulation ***/
	///////////////////////////////////////
	__forceinline void VAddBox(Entity* pEntity, Vec & pos, Quaternion & rot, Vec & dimensions, float r32Mass, string & physMat, bool isStatic) override {}
	__forceinline void VAddPlane(Entity* pEntity, Vec & pos, Quaternion & rot, Vec & halfDimensions, float r32Mass, string & physMat, bool isStatic) override {}
	__forceinline void VAddSphere(Entity* pEntity, Vec & pos, Quaternion & rot, float r32Radius, float r32Mass, string & physMat, bool isStatic) override {}
	//TODO:Add more shapes!!!

	__forceinline void VAddCharacter(Entity* pEntity, Vec & pos, Quaternion & rot, const Vec & dim, float mass, float maxForce, float maxSlope) override {}

	/////////////////////////////////////
	/*** Apply actions to the bodies ***/
	/////////////////////////////////////

	__forceinline void VApplyForce(EntityId entityId, float deltaTime, const Vec & dir, float newtons) override {}
	__forceinline void VApplyTorque(EntityId entityId, float deltaTime, const Vec & dir, float newtons) override {}
	__forceinline void VKinematicMovement(EntityId entityId, Mat4x4 & mat) override {}

	/////////////////////////////////////
	//getters
	__forceinline Vec VGetVelocity(EntityId entityId) override { return Vec{ 0.0f, 0.0f, 0.0f, 0.0f }; }
	__forceinline Vec VGetAngularVelocity(EntityId entityId) override { return Vec{ 0.0f, 0.0f, 0.0f, 0.0f }; }
	__forceinline Mat4x4 VGetTransform(EntityId entityId) override { return Mat4x4{}; }

	/////////////////////////////////////
	//setters
	__forceinline void VSetVelocity(EntityId entityId, const Vec & vel) override {}
	__forceinline void VSetAngularVelocity(EntityId entityId, const Vec & angularVel) override {}
};