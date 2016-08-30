#include "stdafx.h"
#include "Vertex.h"
#include "Triangle.h"

Vertex::Vertex()
{
}

Vertex::Vertex(Vector4d const& vec, Color const& clr):
	position(vec), diffuse_color(clr)
{
}
Vertex::Vertex(Vector4d const& vec, Vector4d const& normal, Color const& dif_clr, Color const& spec_clr) :
	position(vec), normal_vector(normal), diffuse_color(dif_clr), specular_color(spec_clr)
{
}


Vertex & Vertex::operator=(Vertex v)
{
	swap_vertex(*this, v);
	return *this;
}

void Vertex::persrectiveDivide()
{
	position.persrectiveDivide();
}

bool Vertex::operator<(Vertex const & vert) const
{
	return position < vert.position;
}

bool Vertex::operator>(Vertex const & vert) const
{
	return position > vert.position;
}

void Vertex::multCoordinate(Matrix4d const & matr)
{
	position = matr * position;
}



Vertex::~Vertex()
{
}

void swap_vertex(Vertex & a, Vertex & b)
{
	std::swap(a.position, b.position);
	std::swap(a.normal_vector, b.normal_vector);
	std::swap(a.diffuse_color, b.diffuse_color);
}
