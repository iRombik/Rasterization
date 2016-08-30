#pragma once
#include "Color.h"
#include "Vector4d.h"
#include "Matrix4d.h"
#include <vector>

struct Triangle;

struct Vertex
{
public:
	Vertex();
	Vertex(Vector4d const& vec, Color const& clr);
	Vertex(Vector4d const& vec, Vector4d const& normal,  Color const& dif_clr, Color const& spec_clr);
	Vertex(Vertex const& v) = default;
	Vertex(Vertex &&v) = default;
	Vertex& operator=(Vertex v);
	void persrectiveDivide();
	bool operator<(Vertex const& vert) const;
	bool operator>(Vertex const& vert) const;
	void multCoordinate(Matrix4d const& matr);
	~Vertex();

	Vector4d position;
	Vector4d normal_vector;
	Color diffuse_color, specular_color;
};


void swap_vertex(Vertex &a, Vertex& b);