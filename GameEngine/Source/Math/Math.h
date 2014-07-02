#pragma once

#include "Quaternion.h"

struct Math
{
	static __forceinline float Sin(float value) { return sinf(value); }
	static __forceinline float Cos(float value) { return cosf(value); }
	static __forceinline float Tan(float value) { return tanf(value); }
	static __forceinline float Atan(float value) { return atanf(value); }
	static __forceinline float Atan2(float y, float x) { return atan2f(y, x); }
	static __forceinline float ASin(float value) { return asinf((value < -1.f) ? -1.f : ((value < 1.f) ? value : 1.f)); }
	static __forceinline float ACos(float value) { return acosf((value < -1.f) ? -1.f : ((value < 1.f) ? value : 1.f)); }
	static __forceinline float Sqrt(float value) { return sqrtf(value); }
	static __forceinline float Pow(float a, float b) { return powf(a, b); }

	static __forceinline float random(float min, float max, float seed);

	static __forceinline float InvSqrt(float f)
	{
		return 1.f / sqrtf(f);
	}

	template<typename T>
	static __forceinline T Abs(const T a)
	{
		return (a > (T)0) ? a : -a;
	}

	template<typename T>
	static __forceinline T Max(const T a, const T b)
	{
		return (a >= b) : a : b;
	}

	template<typename T>
	static __forceinline T Min(const T a, const T b)
	{
		return (a <= b) ? a : b;
	}

	template<typename T>
	static __forceinline T Lerp(const T & a, const T & b, double coeff)
	{
		return (T)(a + (a - b)*coeff);
	}

	template<typename T>
	static __forceinline T Clamp(T value, T min, T max)
	{
		if (value <= min)
			return min;
		if (value >= max)
			return max;

		return value;
	}

	static __forceinline Quaternion Slerp(const Quaternion & q0, const Quaternion & q1);
};

//specialize for floating-point
template<>
__forceinline float Math::Abs(const float a)
{
	return fabsf(a);
}

__forceinline float Math::random(float min, float max, float seed)
{
	srand(seed);

	int diff = max - min;
	return (rand() % diff) + min;
}

__forceinline Quaternion Math::Slerp(const Quaternion & q0, const Quaternion & q1);
{
	auto cosOmega = q0 * q1;

	if (cosOmega < 0.0f)
	{
		q1 = -q1;
		cosOmega *= -1;
	}

	//if very close just use linear interpolation
	float k0, k1;
	if (cosOmega > 0.9999f)
	{
		k0 = 1.0f - t;
		k1 = t;
	}
	else
	{
		auto sinOmega = Math::Sqrt(1.0f - cosOmega * cosOmega);

		auto omega = Math::Atan2(sinOmega, cosOmega);

		auto invSinOmega = 1.0f / sinOmega;

		k0 = Math::Sin((1.0f - t)*omega)*invSinOmega;
		k1 = Math::Sin(t*omega) *invSinOmega;
	}

	return Quaternion{ q0.x*k0 + q1.x*k1, q0.y*k0 + q1.y*k1, q0.z*k0 + q1.z*k1, q0.w*k0 + q1.w*k1 };
}