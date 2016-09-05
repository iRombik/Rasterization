#include "stdafx.h"
#include "Matrix4d.h"
#include <cmath>

Matrix4d::Matrix4d() : matrix_data{
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
}
{
}

Matrix4d::Matrix4d(float matrix[4][4])
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			matrix_data[i][j] = matrix[i][j];
		}
	}
}

Vector4d Matrix4d::operator*(Vector4d const & vec) const
{
	float result[4];
	for (int i = 0; i < 4; ++i) {
		result[i] = 0;
		for (int j = 0; j < 4; ++j) {
			result[i] += matrix_data[i][j] * vec.coordinate[j];
		}
	}
	return Vector4d(result);
}

Matrix4d Matrix4d::operator*(Matrix4d const & mat) const
{
	Matrix4d result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				result.matrix_data[i][j] += matrix_data[i][k] * mat.matrix_data[k][j];
			}
		}
	}
	return result;
}

void Matrix4d::SetDisplacementVector(Vector4d const & displace_vector)
{
	matrix_data[0][3] = displace_vector.x;
	matrix_data[1][3] = displace_vector.y;
	matrix_data[2][3] = displace_vector.z;
}


Matrix4d Matrix4d::FindInverseMatrix() const
{
	float inverse_matrix[4][4];
	float matrix_of_cofactors[3][3];
	FindMatrixOfCofactors(matrix_of_cofactors);
	//float determinant = FindDeterminantOfMatrix(matrix_of_cofactors);
	TransformMatrix3x3(matrix_of_cofactors);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			//matrix_of_cofactors[i][j] /= determinant;
			inverse_matrix[i][j] = matrix_of_cofactors[i][j];
		}
	}
	inverse_matrix[3][3] = 1;
	Vector4d offset(matrix_data[0][3], matrix_data[1][3], matrix_data[2][3], 0);
	Matrix4d result(inverse_matrix);
	result.SetDisplacementVector(result * offset * -1);
	return result;
}

float Matrix4d::FindDeterminantOfMatrix(float matrix_of_cofactors[3][3]) const
{
	float determinant = 0.0f;
	for (int i = 0; i < 3; ++i) {
		determinant += matrix_data[0][i] * matrix_of_cofactors[0][i] ;
	}
	return determinant;
}

void Matrix4d::FindMatrixOfCofactors(float matrix_of_cofactors[3][3]) const
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			int i1 = (i + 1) % 3;
			int i2 = (i + 2) % 3;
			int j1 = (j + 1) % 3;
			int j2 = (j + 2) % 3;
			matrix_of_cofactors[i][j] = matrix_data[i1][j1] * matrix_data[i2][j2] -
				matrix_data[i1][j2] * matrix_data[i2][j1];
		}
	}
}

void Matrix4d::TransformMatrix3x3(float matrix[3][3]) const
{
	std::swap(matrix[0][1], matrix[1][0]);
	std::swap(matrix[0][2], matrix[2][0]);
	std::swap(matrix[2][1], matrix[1][2]);
}

Matrix4d Matrix4d::perspectiveProjectionMatrix(float alpha, float beta, float near, float far)
{
	near = -near;
	far = -far;
	float e = 1 / tanf(alpha / 2);
	float a = e * tanf(beta / 2);
	float left = -near / e, right = near / e, bot = left * a, top = right * a;
	float  result[4][4] = {
		{ 2.0f * near / (right - left), 0, (right + left) / (right - left) , 0 },
		{ 0, 2.0f * near / (top - bot), (top + bot) / (top - bot) , 0},
		{ 0, 0, -(far + near) / (far - near), - 2.0f * near * far / (far - near) },
		{ 0, 0, -1, 0 }
	};
	return Matrix4d(result);
}

Matrix4d Matrix4d::ortogonalProjectionMatrix(float left, float right, float bot, float top, float near, float far)
{

	float result[4][4] = {
		{ 2.0f / (right - left), 0, 0, -(right + left) / (right - left) },
		{ 0, 2.0f / (top - bot), 0, -(top + bot) / (top - bot) },
		{ 0, 0, 2.0f / (far - near), -(near + far) / (far - near) },
		{ 0, 0, 0, 1 }
	};

	return Matrix4d(result);
}

Matrix4d Matrix4d::projectionToScreen(float w, float h)
{
	float result[4][4] = {
		{h/2, 0, 0, w/2},
		{0, -h/2, 0, h/2},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};
	return Matrix4d(result);
}

Matrix4d Matrix4d::rotateAboutVector(Vector4d const & vec, float alpha)
{
	float cos = std::cosf(alpha), sin = std::sinf(alpha);
	float cos_ = 1 - cos, sin_ = 1 - sin;
	float result[4][4] = {
		{cos + cos_ * vec.coordinate[0] * vec.coordinate[0],
		cos_ * vec.coordinate[0] * vec.coordinate[1] - sin * vec.coordinate[2] ,
		cos_ * vec.coordinate[0] * vec.coordinate[2] + sin * vec.coordinate[1] ,
		0},

		{cos_ * vec.coordinate[0] * vec.coordinate[1] + sin * vec.coordinate[2],
		cos + cos_ * vec.coordinate[1] * vec.coordinate[1] ,
		cos_ * vec.coordinate[1] * vec.coordinate[2] - sin * vec.coordinate[0] ,
		0},

		{cos_ * vec.coordinate[0] * vec.coordinate[2] - sin * vec.coordinate[1],
		cos_ * vec.coordinate[1] * vec.coordinate[2] + sin * vec.coordinate[0],
		cos + cos_ * vec.coordinate[2] * vec.coordinate[2],
		0},

		{0,
		0,
		0,
		1}
	};
	return Matrix4d(result);
}

Matrix4d Matrix4d::rotateAboutX(float alpha)
{
	float cos = std::cosf(alpha), sin = std::sinf(alpha);
	float result[4][4] = {
		{ 1, 0, 0, 0 },
		{ 0, cos, -sin, 0 },
		{ 0, sin, cos, 0 },
		{ 0, 0, 0, 1 }
	};
	return Matrix4d(result);
}

Matrix4d Matrix4d::rotateAboutY(float alpha)
{
	float cos = std::cosf(alpha), sin = std::sinf(alpha);
	float result[4][4] = {
		{ cos, 0, -sin, 0 },
		{ 0, 1, 0, 0 },
		{ sin, 0, cos, 0 },
		{ 0, 0, 0, 1 }
	};
	return Matrix4d(result);
}

Matrix4d Matrix4d::rotateAboutZ(float alpha)
{
	float cos = std::cosf(alpha), sin = std::sinf(alpha);
	float result[4][4] = {
		{ cos, -sin, 0, 0},
		{ sin, cos, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	return Matrix4d(result);
}



Matrix4d::~Matrix4d()
{
}
