#include "stdafx.h"
#include "Cube.h"

const int Cube::pos_for_triangle[12][3] = {
	{ 1, 3, 2 },{ 1, 2, 0 }, //-x
	{ 4, 7, 5 },{ 4, 6, 7 }, //x
	{ 1, 0, 4 },{ 1, 4, 5 }, //-y
	{ 7, 6, 2 },{ 7, 2, 3 }, //y
	{ 4, 2, 6 },{ 4, 0, 2 }, //-z
	{ 1, 7, 3 },{ 1, 5, 7 } //z
};


Cube::Cube()
{
}


Cube::Cube(Vector4d const& z_vec, Vector4d const& center_vec, float rad):
	z_vector(z_vec), offset_vector(center_vec), radius(rad)
{
}


void Cube::calulateCenterMatrix()
{
	Vector4d y_vector(0, 1, 0, 0);
	Vector4d x_vector(help_functions::crossProduct(y_vector, z_vector));
	y_vector = help_functions::crossProduct(z_vector, x_vector);

	x_vector.normalize();
	y_vector.normalize();
	z_vector.normalize();
	float center_coordinate[4][4]{
		{ x_vector.x, x_vector.y, x_vector.z, offset_vector.coordinate[0] },
		{ y_vector.x, y_vector.y, y_vector.z, offset_vector.coordinate[1] },
		{ z_vector.x, z_vector.y, z_vector.z, offset_vector.coordinate[2] },
		{ 0, 0, 0, 1 }
	};
	this->center = center_coordinate;
}

void Cube::rotateCube(Matrix4d const & rotate_mat)
{
	center = center * rotate_mat;
}

void Cube::generateVertices(Vector4d* vertex_set) const
{
	Vector4d x(radius, 0, 0, 0);
	Vector4d y(0, radius, 0, 0);
	Vector4d z(0, 0, radius, 0);
	Vector4d w(0, 0, 0, 1);

	x = center * x;
	y = center * y;
	z = center * z;
	w = center * w;

	//create cube verticles
	int index = 0;
	for (float i = -1; i <= 1; i += 2) {
		for (float j = -1; j <= 1; j += 2) {
			for (float k = -1; k <= 1; k += 2) {
				vertex_set[index++] = x * i + y * j + z * k + w;
			}
		}
	}
}

void Cube::giveTriangles(Triangle pointer_to_set_of_triangle[12]) const
{
	Vector4d vertex_set[8];
	generateVertices(vertex_set);

	for (int i = 0; i < 12; ++i) {
		int a = pos_for_triangle[i][0], b = pos_for_triangle[i][1], c = pos_for_triangle[i][2];
		pointer_to_set_of_triangle[i] = Triangle(vertex_set[a], vertex_set[b], vertex_set[c]);
	}
}


Cube::~Cube()
{
}
