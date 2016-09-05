#include "stdafx.h"
#include "Vector4d.h"

bool Vector4d::operator<(Vector4d const & vec) const
{
	return (y < vec.y) ||
		((y == vec.y) && (x < vec.x));
}

bool Vector4d::operator>(Vector4d const & vec) const
{
	return (y > vec.y) ||
		((y == vec.y) && (x > vec.x));
}


void Vector4d::persrectiveDivide()
{
	float w_inv = 1 / w;
	for (int i = 0; i < 3; ++i) {
		coordinate[i] *= w_inv;
	}
	w = w_inv;
}


Vector4d::~Vector4d()
{
}

