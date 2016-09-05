#pragma once
#include "Vector4d.h"
#include "Matrix4d.h"
#include "help_functions.h"

struct Triangle
{
	Triangle();
	Triangle(Vector4d const& a, Vector4d const& b, Vector4d const& c);
	Triangle(Vector4d const tmp_vertices[3]);
	void TransformTriangle(Matrix4d const& transform_matrix);
	void CalculateNormalVector();

	~Triangle();

	Vector4d normal_vector;
	Vector4d vertices[3];
};

