#pragma once
#include "Vertex.h" 
#include "Triangle.h"
#include "Matrix4d.h"
#include "LightSource.h"
#include "help_functions.h"

struct Cube
{
	Cube();
	Cube(Vector4d z_vector, Vector4d center, float rad);
	void calulateCenterMatrix();
	void rotateCube(Matrix4d const& rotate_matrix);
	void generateVertices(Vector4d* vertex_set) const;
	void giveTriangles(Triangle pointer_to_set_of_triangle[12]) const;
	~Cube();

	Vector4d offset_vector, z_vector;
	Matrix4d center;
	float radius;

	static const int pos_for_triangle[12][3];
};

