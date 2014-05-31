#pragma once

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