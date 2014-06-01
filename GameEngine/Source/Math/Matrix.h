#pragma once

#include "Vector.h"

class Mat4x4
{
public:
	union
	{
		float m[4][4];
	};

	static const Mat4x4 Identity;

	/*
	*Default Constructor
	*/
	__forceinline Mat4x4();

	__forceinline Mat4x4(const Vec & v1, const Vec & v2, const Vec & v3, const Vec & v4);

	__forceinline void SetIdentity();

	__forceinline Mat4x4 & operator*=(const Mat4x4 & other);

	__forceinline Mat4x4 & operator*=(float other);

	__forceinline Mat4x4 & operator+=(const Mat4x4 & other);

	__forceinline Mat4x4 & operator-=(const Mat4x4 & other);

	__forceinline bool operator==(const Mat4x4 & other) const;

	__forceinline bool operator!=(const Mat4x4 & other) const;
};

__forceinline Mat4x4 & Mat4x4::operator*=(const Mat4x4 & other)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] *= other.m[i][j];
		}
	}

	return *this;
}

__forceinline Mat4x4 & Mat4x4::operator+=(const Mat4x4 & other)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] += other.m[i][j];
		}
	}

	return *this;
}

__forceinline Mat4x4 & Mat4x4::operator-=(const Mat4x4 & other)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] -= other.m[i][j];
		}
	}

	return *this;
}

__forceinline Mat4x4 & Mat4x4::operator*=(float other)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] *= other;
		}
	}

	return *this;
}

__forceinline Mat4x4 operator*(Mat4x4 m1, const Mat4x4 & m2)
{
	m1 *= m2;
	return m1;
}

__forceinline Mat4x4 operator*(Mat4x4 m, float other)
{
	m *= other;
	return m;
}

__forceinline Mat4x4 operator+(Mat4x4 m1, const Mat4x4 & m2)
{
	m1 += m2;
	return m1;
}

__forceinline Mat4x4 operator-(Mat4x4 m1, const Mat4x4 & m2)
{
	m1 -= m2;
	return m1;
}

__forceinline bool Mat4x4::operator==(const Mat4x4 & other) const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m[i][j] != other.m[i][j])
			{
				return false;
			}
		}
	}

	return true;
}

__forceinline bool Mat4x4::operator!=(const Mat4x4 & other) const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m[i][j] != other.m[i][j])
			{
				return true;
			}
		}
	}

	return false;
}

class MatrixStack
{
private:
	vector<Mat4x4> m_matrices;
	unsigned char m_count;

public:

	__forceinline MatrixStack();

	//Get top(current) matrix
	__forceinline const Mat4x4 & Get() const;

	//Get number of matrices in the stack
	__forceinline unsigned char Count() const;

	//remove current matrix
	__forceinline void Pop();

	//Add transform to the current matrix and add new one
	__forceinline void PushTransform(const Mat4x4 & mat);
};

__forceinline MatrixStack::MatrixStack()
	: m_count{ 1 }
{
	m_matrices.reserve(10);
	m_matrices.emplace_back();
}

//Get top(current) matrix
__forceinline const Mat4x4 & MatrixStack::Get() const
{
	return m_matrices.back();
}

//Get number of matrices in the stack
__forceinline unsigned char MatrixStack::Count() const
{
	return m_count;
}

//remove current matrix
__forceinline void MatrixStack::Pop()
{
	m_matrices.pop_back();
	--m_count;
}

//Add transform to the current matrix and add new one
__forceinline void MatrixStack::PushTransform(const Mat4x4 & mat)
{
	m_matrices.push_back(m_matrices.back() * mat);
	++m_count;
}