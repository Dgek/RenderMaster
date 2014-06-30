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

	__forceinline static Mat4x4 CreateViewMatrixLH(const Vec & pos, const Vec & lookDir, const Vec & up);
	__forceinline static Mat4x4 CreatePerspectiveProjectionLH(float fov, float aspect, float nearZ, float farZ);
	__forceinline static Mat4x4 CreateOrthoProjectionLH(float width, float height, float nearZ, float farZ);
	__forceinline static Mat4x4 CreateTranslation(const Vec & vec);
	__forceinline static Mat4x4 CreateRotationX(float angleInRadians);
	__forceinline static Mat4x4 CreateRotationY(float angleInRadians);
	__forceinline static Mat4x4 CreateRotationZ(float angleInRadians);
	__forceinline static Mat4x4 CreateScaling(const Vec & scale);
	__forceinline static Mat4x4 CreateScaling(float x, float y, float z);
	__forceinline static Mat4x4 CreateRollPitchYaw(float rollInRadians, float pitchInRadians, float yawInRadians);


	/*
	*Default Constructor
	*/
	__forceinline Mat4x4();

	__forceinline Mat4x4(float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44);

	__forceinline Mat4x4(const Vec & v1, const Vec & v2, const Vec & v3, const Vec & v4);

	__forceinline void SetIdentity();

	__forceinline Mat4x4 GetTransposed() const;

	__forceinline void Transpose();

	__forceinline Mat4x4 & operator*=(const Mat4x4 & other);

	__forceinline Mat4x4 & operator*=(float other);

	__forceinline Mat4x4 & operator+=(const Mat4x4 & other);

	__forceinline Mat4x4 & operator-=(const Mat4x4 & other);

	__forceinline bool operator==(const Mat4x4 & other) const;

	__forceinline bool operator!=(const Mat4x4 & other) const;
};

__forceinline Mat4x4::Mat4x4()
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

__forceinline Mat4x4::Mat4x4(float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44)
{
	m[0][0] = m11; m[0][1] = m12; m[0][2] = m13; m[0][3] = m14;
	m[1][0] = m21; m[1][1] = m22; m[1][2] = m23; m[1][3] = m24;
	m[2][0] = m31; m[2][1] = m32; m[2][2] = m33; m[2][3] = m34;
	m[3][0] = m41; m[3][1] = m42; m[3][2] = m43; m[3][3] = m44;
}

__forceinline Mat4x4::Mat4x4(const Vec & v1, const Vec & v2, const Vec & v3, const Vec & v4)
{
	m[0][0] = v1.x; m[0][1] = v1.y; m[0][2] = v1.z; m[0][3] = v1.w;
	m[1][0] = v2.x; m[1][1] = v2.y; m[1][2] = v2.z; m[1][3] = v2.w;
	m[2][0] = v3.x; m[2][1] = v3.y; m[2][2] = v3.z; m[2][3] = v3.w;
	m[3][0] = v4.x; m[3][1] = v4.y; m[3][2] = v4.z; m[3][3] = v4.w;
}

__forceinline Vec operator*(const Vec & v, const Mat4x4 & M)
{
	return Vec(v.x * M.m[0][0] + v.y * M.m[1][0] + v.z * M.m[2][0] + v.w * M.m[3][0],
		v.x * M.m[0][1] + v.y * M.m[1][1] + v.z * M.m[2][1] + v.w * M.m[3][1],
		v.x * M.m[0][2] + v.y * M.m[1][2] + v.z * M.m[2][2] + v.w * M.m[3][2],
		v.x * M.m[0][3] + v.y * M.m[1][3] + v.z * M.m[2][3] + v.w * M.m[3][3]);
}

__forceinline Mat4x4 & Mat4x4::operator*=(const Mat4x4 & other)
{
	Vec row0 = Vec{ m[0][0], m[0][1], m[0][2], m[0][3] } *other;
	Vec row1 = Vec{ m[1][0], m[1][1], m[1][2], m[1][3] } *other;
	Vec row2 = Vec{ m[2][0], m[2][1], m[2][2], m[2][3] } *other;
	Vec row3 = Vec{ m[3][0], m[3][1], m[3][2], m[3][3] } *other;

	m[0][0] = row0.x; m[0][1] = row0.y; m[0][2] = row0.z; m[0][3] = row0.w;
	m[1][0] = row1.x; m[1][1] = row1.y; m[1][2] = row1.z; m[1][3] = row1.w;
	m[2][0] = row2.x; m[2][1] = row2.y; m[2][2] = row2.z; m[2][3] = row2.w;
	m[3][0] = row3.x; m[3][1] = row3.y; m[3][2] = row3.z; m[3][3] = row3.w;

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


__forceinline Mat4x4 Mat4x4::CreateViewMatrixLH(const Vec & pos, const Vec & lookDir, const Vec & up)
{
	Vec & zaxis = Vec::Normalize(lookDir);
	Vec & xaxis = Vec::Normalize(up ^ zaxis);
	Vec & yaxis = zaxis ^ xaxis;

	return Mat4x4{	xaxis.x, yaxis.x, zaxis.x, 0.0f,
					xaxis.y, yaxis.y, zaxis.y, 0.0f,
					xaxis.z, yaxis.z, zaxis.z, 0.0f,
					-Dot4(xaxis, pos), -Dot4(yaxis, pos), -Dot4(zaxis, pos), 1.0f };
}

__forceinline Mat4x4 Mat4x4::CreatePerspectiveProjectionLH(float fov, float aspect, float nearZ, float farZ)
{
	float SinFov = Math::Sin(fov * 0.5f);
	float CosFov = Math::Cos(fov * 0.5f);

	float height = CosFov / SinFov;
	float width = height / aspect;

	return Mat4x4{	width, 0.0f, 0.0f, 0.0f,
					0.0f, height, 0.0f, 0.0f,
					0.0f, 0.0f, farZ / (farZ - nearZ), 1.0f,
					0.0f, 0.0f, -nearZ*farZ / (farZ - nearZ), 0.0f };
}

__forceinline Mat4x4 Mat4x4::CreateOrthoProjectionLH(float width, float height, float nearZ, float farZ)
{
	return Mat4x4{	2.0f / width, 0.0f, 0.0f, 0.0f,
					0.0f, 2.0f / height, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f / (farZ - nearZ), 0.0f,
					0.0f, 0.0f, nearZ / (nearZ - farZ), 1.0f };
}

__forceinline Mat4x4 Mat4x4::CreateTranslation(const Vec & vec)
{
	return Mat4x4{	1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					vec.x, vec.y, vec.z, vec.w };
}

__forceinline Mat4x4 Mat4x4::CreateRotationX(float angleInRadians)
{
	return Mat4x4{ 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, Math::Cos(angleInRadians), Math::Sin(angleInRadians), 0.0f,
		0.0f, -Math::Sin(angleInRadians), Math::Cos(angleInRadians), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
}

__forceinline Mat4x4 Mat4x4::CreateRotationY(float angleInRadians)
{
	return Mat4x4{	Math::Cos(angleInRadians), 0.0f, -Math::Sin(angleInRadians), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		Math::Sin(angleInRadians), 0.0f, Math::Cos(angleInRadians), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
}

__forceinline Mat4x4 Mat4x4::CreateRotationZ(float angleInRadians)
{
	return Mat4x4{ Math::Cos(angleInRadians), Math::Sin(angleInRadians), 0.0f, 0.0f,
		-Math::Sin(angleInRadians), Math::Cos(angleInRadians), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
}

__forceinline Mat4x4 Mat4x4::CreateScaling(const Vec & scale)
{
	return Mat4x4{ scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
}

__forceinline Mat4x4 Mat4x4::CreateScaling(float x, float y, float z)
{
	return Mat4x4{ x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
}

__forceinline Mat4x4 Mat4x4::CreateRollPitchYaw(float rollInRadians, float pitchInRadians, float yawInRadians)
{
	return CreateRotationZ(rollInRadians) * CreateRotationX(pitchInRadians) * CreateRotationY(yawInRadians);
}

__forceinline Mat4x4 Mat4x4::GetTransposed() const
{
	Mat4x4 mat;

	mat.m[0][0] = m[0][0];
	mat.m[0][1] = m[1][0];
	mat.m[0][2] = m[2][0];
	mat.m[0][3] = m[3][0];

	mat.m[1][0] = m[0][1];
	mat.m[1][1] = m[1][1];
	mat.m[1][2] = m[2][1];
	mat.m[1][3] = m[3][1];

	mat.m[2][0] = m[0][2];
	mat.m[2][1] = m[1][2];
	mat.m[2][2] = m[2][2];
	mat.m[2][3] = m[3][2];

	mat.m[3][0] = m[0][3];
	mat.m[3][1] = m[1][3];
	mat.m[3][2] = m[2][3];
	mat.m[3][3] = m[3][3];

	return mat;
}

__forceinline void Mat4x4::Transpose()
{
	Mat4x4 temp = GetTransposed();
	*this = temp;
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