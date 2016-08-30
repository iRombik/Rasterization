#pragma once
#include "Triangle.h"
#include "Vertex.h"
#include "help_functions.h"

struct ComplexTriangle :
	public Triangle
{
	ComplexTriangle();
	ComplexTriangle(Triangle const& triangle);
	ComplexTriangle& operator=(Triangle const& triangle);
	void calculateBarycenticCoodinates(Vector4d const & dot_vector, float triangle_area,
		float* barycentric_coordinates) const;

	~ComplexTriangle();

	Vertex complex_vertices[3];
};

