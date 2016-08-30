#include "stdafx.h"
#include "ComplexTriangle.h"


ComplexTriangle::ComplexTriangle()
{
}

ComplexTriangle::ComplexTriangle(Triangle const & triangle) : Triangle(triangle)
{
	for (int i = 0; i < 3; ++i) {
		complex_vertices[i].position = vertices[i];
	}
}

ComplexTriangle & ComplexTriangle::operator=(Triangle const & triangle)
{
	for (int i = 0; i < 3; ++i) {
		complex_vertices[i].position = triangle.vertices[i];
	}
	return *this;
}


void ComplexTriangle::calculateBarycenticCoodinates(Vector4d const & dot_vector, float triangle_area,
	float* barycentric_coordinates) const
{
	float area0 = help_functions::calculateArea(
		complex_vertices[1].position,
		complex_vertices[2].position,
		dot_vector);

	float area1 = help_functions::calculateArea(
		complex_vertices[2].position,
		complex_vertices[0].position,
		dot_vector);

	//calculate barycentic coordinates
	barycentric_coordinates[0] = area0 / triangle_area;
	barycentric_coordinates[1] = area1 / triangle_area;
	barycentric_coordinates[2] = 1.0f - barycentric_coordinates[0] - barycentric_coordinates[1];
}


ComplexTriangle::~ComplexTriangle()
{
}
