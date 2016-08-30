#pragma once
#include "Vector4d.h"
#include "Pixel.h"

struct Triangle;

namespace help_functions {
	enum
	{
		X, Y, Z, W
	};

	inline float calculateArea(Vector4d const &a, Vector4d const &b, Vector4d const &c)
	{
		return a.coordinate[X] * (b.coordinate[Y] - c.coordinate[Y]) +
			b.coordinate[X] * (c.coordinate[Y] - a.coordinate[Y]) +
			c.coordinate[X] * (a.coordinate[Y] - b.coordinate[Y]);
	}

	inline Vector4d crossProduct(Vector4d const & a, Vector4d const & b)
	{
		return Vector4d(a.coordinate[Y] * b.coordinate[Z] - b.coordinate[Y] * a.coordinate[Z],
			b.coordinate[X] * a.coordinate[Z] - a.coordinate[X] * b.coordinate[Z],
			a.coordinate[X] * b.coordinate[Y] - a.coordinate[Y] * b.coordinate[X],
			0);
	}

	inline float dotProduct(Vector4d const &a, Vector4d const &b)
	{
		return a.coordinate[X] * b.coordinate[X] +
			a.coordinate[Y] * b.coordinate[Y] +
			a.coordinate[Z] * b.coordinate[Z];
	}

	void sortByYCoordinate(Vector4d & a, Vector4d & b, Vector4d & c);

	Pixel* rasterizeLine(
		Vector4d const& first_point_of_segment, Vector4d const& second_point_of_segment,
		Pixel *pixels_belonging_to_line
		);

	void getBorderPixelsOfTriangle(Pixel* left_border, Pixel* right_border, Triangle & triangle);
}//help_funtions
