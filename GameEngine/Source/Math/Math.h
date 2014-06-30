#pragma once

struct Math
{
	static __forceinline float Sin(float value) { return sinf(value); }
	static __forceinline float Cos(float value) { return cosf(value); }
	static __forceinline float Tan(float value) { return tanf(value); }
	static __forceinline float Atan(float value) { return atanf(value); }
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