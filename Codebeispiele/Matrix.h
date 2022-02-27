#pragma once

#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

inline double to_degree(double radian)
{
	return radian * (180.0 / M_PI);
}

inline double to_radian(double degree)
{
	return degree / (180.0 / M_PI);
}

struct Vector3f
{
	Vector3f(float x, float y, float z)
		: x(x), y(y), z(z) {}

	float x, y, z;
};

struct Matrix4f
{
	float& operator[](int index)
	{
		return values[index];
	}

	Matrix4f& operator*=(const Matrix4f& other)
	{
		return *this = this->operator*(other);
	}

	Matrix4f operator*(const Matrix4f& other)
	{
		Matrix4f newMatrix(0.0f);

		newMatrix[0]  = values[0] * other.values[0] + values[1] * other.values[4] + values[2] * other.values[8] + values[3] * other.values[12];
		newMatrix[1]  = values[0] * other.values[1] + values[1] * other.values[5] + values[2] * other.values[9] + values[3] * other.values[13];
		newMatrix[2]  = values[0] * other.values[2] + values[1] * other.values[6] + values[2] * other.values[10] + values[3] * other.values[14];
		newMatrix[3]  = values[0] * other.values[3] + values[1] * other.values[7] + values[2] * other.values[11] + values[3] * other.values[15];


		newMatrix[4]  = values[4] * other.values[0] + values[5] * other.values[4] + values[6] * other.values[8] + values[7] * other.values[12];
		newMatrix[5]  = values[4] * other.values[1] + values[5] * other.values[5] + values[6] * other.values[9] + values[7] * other.values[13];
		newMatrix[6]  = values[4] * other.values[2] + values[5] * other.values[6] + values[6] * other.values[10] + values[7] * other.values[14];
		newMatrix[7]  = values[4] * other.values[3] + values[5] * other.values[7] + values[6] * other.values[11] + values[7] * other.values[15];

		newMatrix[8]  = values[8] * other.values[0] + values[9] * other.values[4] + values[10] * other.values[8] + values[11] * other.values[12];
		newMatrix[9]  = values[8] * other.values[1] + values[9] * other.values[5] + values[10] * other.values[9] + values[11] * other.values[13];
		newMatrix[10] = values[8] * other.values[2] + values[9] * other.values[6] + values[10] * other.values[10] + values[11] * other.values[14];
		newMatrix[11] = values[8] * other.values[3] + values[9] * other.values[7] + values[10] * other.values[11] + values[11] * other.values[15];

		newMatrix[12] = values[12] * other.values[0] + values[13] * other.values[4] + values[14] * other.values[8] + values[15] * other.values[12];
		newMatrix[13] = values[12] * other.values[1] + values[13] * other.values[5] + values[14] * other.values[9] + values[15] * other.values[13];
		newMatrix[14] = values[12] * other.values[2] + values[13] * other.values[6] + values[14] * other.values[10] + values[15] * other.values[14];
		newMatrix[15] = values[12] * other.values[3] + values[13] * other.values[7] + values[14] * other.values[11] + values[15] * other.values[15];

		return newMatrix;
	}

	Matrix4f(float value)
	{
		for (int i = 0; i < 16; i++)
		{
			values[i] = 0;
		}
	}

public:
	float values[16];
};

Matrix4f identity()
{
	Matrix4f matrix(0.0f);

	matrix[0] = 1.0f;
	matrix[5] = 1.0f;
	matrix[10] = 1.0f;
	matrix[15] = 1.0f;

	return matrix;
}

Matrix4f translate(float x, float y, float z)
{
	Matrix4f newMatrix = identity();

	newMatrix[12] = x;
	newMatrix[13] = y;
	newMatrix[14] = z;

	return newMatrix;
}

Matrix4f inverse(const Matrix4f& matrix)
{
	//   2 * x + 3 * y = 1
	//   4 * x + 7 * y = 0

	//   2 * z + 3 * w = 0
	//	 2 * z + 3 * w = 1

	// 2 3 1
	// 4 7 0

	// 2 3 1
	// 0 1 -2

	// 1 1,5 0,5
	// 0 1   -2

	// 1 0 3,5
	// 0 1 -2

	// 7 + (-6) = 1
	
	return Matrix4f(0.0f);

}

Matrix4f perspective(float width, float height, int fov, float farplane, float nearplane)
{
	Matrix4f matrix(0.0f);

	float aspect = width / height;

	float y = cos(0.5 * fov) / sin(0.5 * fov);
	float x = y * height / width;
	
	matrix[0] = x;
	matrix[5] = y;
	matrix[10] = farplane / (farplane - nearplane);
	matrix[11] = 1;
	matrix[14] = -(farplane * nearplane) / (farplane - nearplane);
	
	return matrix;
}

Matrix4f translate(const Vector3f& vec)
{
	return translate(vec.x, vec.y, vec.z);
}

Matrix4f scale(const Vector3f& vec)
{
	Matrix4f newMatrix = identity();

	newMatrix[0] = vec.x;
	newMatrix[5] = vec.y;
	newMatrix[10] = vec.z;

	return newMatrix;
}

Matrix4f rotate(const Vector3f& axis, float angle)
{
	Matrix4f newMatrix = identity();

	float c = cos(angle);
	float s = sin(angle);

	newMatrix[0] = c + axis.x * axis.x * (1 - c);
	newMatrix[1] = axis.x * axis.y * (1 - c) - axis.z * s;
	newMatrix[2] = axis.x * axis.z * (1 - c) + axis.y * s;
	
	newMatrix[4] = axis.y * axis.x * (1 - c) + axis.z * s;
	newMatrix[5] = c + axis.y * axis.y * (1 - c);
	newMatrix[6] = axis.y * axis.z * (1 - c) - axis.x * s;

	newMatrix[8] = axis.z * axis.x * (1 - c) - axis.y * s;
	newMatrix[9] = axis.z * axis.y * (1 - c) + axis.x * s;
	newMatrix[10] = c + axis.z * axis.z * (1 - c);

	return newMatrix;
}