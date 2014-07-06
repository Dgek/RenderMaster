#pragma once

class float2 final
{
public:
	float x, y;

	/** default constructor **/
	float2();

	/*
	#Constructor
	*
	* @param value to set for all components
	*/
	explicit float2(float value);

	/*
	#Constructor
	*
	* @param inX - x
	* @param inY - y
	*/
	float2(float inX, float inY);

	/*
	*Dot Product of two vectors
	*
	* @param v1 the first vector
	* @param v2 the second vector
	*
	* @return Dot Product
	*/
	//__forceinline static float dot(const float2 & v1, const float2 & v2);
};

__forceinline float2::float2()
	: x{ 0 }, y{ 0 }
{}

__forceinline float2::float2(float value)
	: x{ value }, y{ value }
{}

__forceinline float2::float2(float inX, float inY)
	: x{ inX }, y{ inY }
{}