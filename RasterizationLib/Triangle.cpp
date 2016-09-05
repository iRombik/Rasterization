#include "stdafx.h"
#include "Triangle.h"


Triangle::Triangle()
{
}

Triangle::Triangle(Vector4d const& a, Vector4d const& b, Vector4d const& c) : vertices{a, b, c}
{
}

Triangle::Triangle(Vector4d const tmp_vertices[3])
{
	vertices[0] = tmp_vertices[0];
	vertices[1] = tmp_vertices[1];
	vertices[2] = tmp_vertices[2];
}

void Triangle::TransformTriangle(Matrix4d const & transform_matrix)
{
	vertices[0] = transform_matrix * vertices[0];
	vertices[1] = transform_matrix * vertices[1];
	vertices[2] = transform_matrix * vertices[2];
}


void Triangle::CalculateNormalVector()
{
	normal_vector = help_functions::crossProduct(vertices[1] - vertices[0],
		vertices[2] - vertices[0]);
}


Triangle::~Triangle()
{
}
