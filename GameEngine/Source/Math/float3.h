#pragma once

class float3 final
{
public:
	float x, y, z;

	/** default constructor **/
	__forceinline float3();

	/*
	#Constructor
	*
	* @param value to set for all components
	*/
	explicit __forceinline float3(float value);

	/*
	#Constructor
	*
	* @param inX - x 
	* @param inY - y
	* @param inZ - z
	*/
	__forceinline float3(float inX, float inY, float inZ);

	/*
	*Constructor
	*
	* @param v - 2d vector to copy to x, y
	* @param inZ - z 
	*/
	explicit float3(const float2 & v, float inZ);

	/*
	*Cross Product of tw vectors
	*
	* @param v1 - the first vector
	* @param v2 - the second vector
	*
	* @return Cross Product
	*/
	__forceinline static float3 cross(const float3 & v1, const float3 & v2);

	/*
	*Dot Product of two vectors
	*
	* @param v1 the first vector
	* @param v2 the second vector
	*
	* @return Dot Product
	*/
	__forceinline static float dot(const float3 & v1, const float3 & v2);
};

__forceinline float3 operator^(float3 v1, const float3 & v2)
{
	return float3
	{
	v1.y * v2.z - v1.z * v2.y,
	v1.z * v2.x - v1.x * v2.z,
	v1.x * v2.y - v1.y * v2.x,
};
}

__forceinline float3::float3()
{}

__forceinline float3::float3(float value)
	: x{ value }
{}

__forceinline float3::float3(float inX, float inY, float inZ)
	: x{ inX }, y{ inY }, z{ inZ }
{}

__forceinline float3 float3::cross(const float3 & v1, const float3 & v2)
{
	return v1 ^ v2;
}

/*__forceinline float float3::dot(const float3 & v1, const float3 & v2)
{
	return v1 | v2;
} */