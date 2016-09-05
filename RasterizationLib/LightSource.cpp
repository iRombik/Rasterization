#include "stdafx.h"
#include "LightSource.h"


LightSource::LightSource()
{
}

LightSource::LightSource(Vector4d pos, Color clr, float k0_, float k1_, float k2_):
	position(pos), light_color(clr), k0(k0_), k1(k1_), k2(k2_)
{
}

Color LightSource::calculateIntensivity(Vector4d const& dot) const
{
	float distance = (dot - position).calculateLenOfVector();
	return light_color * (1.0f / (k0 + k1 * distance + k2 * distance * distance));
}

void LightSource::calculateShading(Vector4d const& vector_to_point, Vector4d const& normal_vector, float material_specular_exponent,
	Color& diffuse_term, Color& specular_term) const
{
	Vector4d vector_to_light = position - vector_to_point;
	vector_to_light.normalize();
	Color intens = calculateIntensivity(vector_to_point);
	float diffuse_coefficient = std::fmaxf(help_functions::dotProduct(vector_to_light, normal_vector), 0.0);
	diffuse_term = intens * diffuse_coefficient;

	if (diffuse_coefficient <= 0) {
		specular_term = Color(0, 0, 0);
	}
	else {
		Vector4d vector_to_viewer = vector_to_point * -1;
		vector_to_viewer.normalize();
		Vector4d halfway_vector = vector_to_light + vector_to_viewer;
		halfway_vector.normalize();
		specular_term =
			intens * std::powf(
				std::fmax(help_functions::dotProduct(halfway_vector, normal_vector), 0.0f),
				material_specular_exponent
			);
	}
}

Vector4d const & LightSource::getPosition() const
{
	return position;
}


LightSource::~LightSource()
{
}
