#pragma once
#include "Vertex.h"
#include "Color.h"
#include "help_functions.h"

class LightSource
{
public:
	LightSource();
	LightSource(Vector4d pos, Color clr = Color(1.0f, 1.0f, 1.0f), float k0_ = 1.0, float k1_ = 0.0, float k2_ = 0.0);
	Color calculateIntensivity(Vector4d const& dot) const;
	void calculateShading(Vector4d const& vector_to_point, Vector4d const& normal_vector,
		float material_shininess,
		Color& diffuse_term, Color& specular_term) const;
	Vector4d const& getPosition() const;
	~LightSource();

private:
	Vector4d position;
	float k0, k1, k2;
	Color light_color;
};

