#pragma once
#include "Vector4d.h"
#include <vector>

struct Matrix4d
{
public:
	Matrix4d();
	Matrix4d(float matrix[4][4]);
	Vector4d operator*(Vector4d const&) const;
	Matrix4d operator*(Matrix4d const& mat) const;
	static Matrix4d perspectiveProjectionMatrix(float alpha, float beta, float n, float f);
	static Matrix4d ortogonalProjectionMatrix(float l, float r, float b, float t, float n, float f);
	static Matrix4d projectionToScreen(float w, float h);
	static Matrix4d rotateAboutVector(Vector4d const& vec, float alpha);
	static Matrix4d rotateAboutX(float alpha);
	static Matrix4d rotateAboutZ(float alpha);
	~Matrix4d();

	float matrix_data[4][4];
};

