#pragma once

#include "Matrix.h"

class Quaternion
{
public:

	float x, y, z, w;

	__forceinline Quaternion();

	__forceinline Quaternion(const Quaternion & q);

	__forceinline Quaternion(float inX, float inY, float inZ, float inW);

	/*
	* Constructor
	* 
	* @param M a rotation matrix
	*/
	explicit Quaternion(const Mat4x4 & M);

};

__forceinline Quaternion::Quaternion()
	: x{ 0.0f }, y{ 0.0f }, z{ 0.0f }, w{ 1.0f }
{}

__forceinline Quaternion::Quaternion(float inX, float inY, float inZ, float inW)
	: x{ inX }, y{ inY }, z{ inZ }, w{ inW }
{}