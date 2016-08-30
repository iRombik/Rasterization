#pragma once
#include <vector>

struct Vector4d
{
	Vector4d::Vector4d();
	Vector4d(float coord_vector[4]);
	Vector4d(float x, float y, float z, float w);

	Vector4d(Vector4d const& tmp_vector) = default;
	Vector4d(Vector4d && tmp_vector) = default;

	bool operator<(Vector4d const& vec) const;
	bool operator>(Vector4d const& vec) const;
	Vector4d operator-(Vector4d const & vec) const;
	Vector4d Vector4d::operator+(Vector4d const & vec) const;
	Vector4d Vector4d::operator*(float f) const;
	Vector4d & Vector4d::operator=(Vector4d const & vec);
	Vector4d & Vector4d::operator+=(Vector4d const & vec);

	float Vector4d::calculateLenOfVector() const;
	void normalize();
	void persrectiveDivide();

	~Vector4d();

	union
	{
		struct {
			float x, y, z, w;
		};
		float coordinate[4];
	};
};



inline Vector4d::Vector4d() : coordinate{ 0, 0, 0, 0 }
{
}

inline Vector4d::Vector4d(float coord_vector[4])
{
	coordinate[0] = coord_vector[0];
	coordinate[1] = coord_vector[1];
	coordinate[2] = coord_vector[2];
	coordinate[3] = coord_vector[3];
}

inline Vector4d::Vector4d(float x, float y, float z, float w) : coordinate{ x, y, z, w }
{
}


inline Vector4d Vector4d::operator-(Vector4d const & vec) const
{
	float result_x, result_y, result_z, result_w;
	result_x = x - vec.x;
	result_y = y - vec.y;
	result_z = z - vec.z;
	result_w = w - vec.w;
	return Vector4d(result_x, result_y, result_z, result_w);
}

inline Vector4d Vector4d::operator+(Vector4d const & vec) const
{
	float result_x, result_y, result_z, result_w;
	result_x = x + vec.x;
	result_y = y + vec.y;
	result_z = z + vec.z;
	result_w = w + vec.w;
	return Vector4d(result_x, result_y, result_z, result_w);
}

inline Vector4d Vector4d::operator*(float f) const
{
	float result_x = x * f;
	float result_y = y * f;
	float result_z = z * f;
	float result_w = w * f;
	return Vector4d(result_x, result_y, result_z, result_w);
}

inline Vector4d & Vector4d::operator=(Vector4d const & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
	return *this;
}

inline Vector4d & Vector4d::operator+=(Vector4d const & vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}

inline float Vector4d::calculateLenOfVector() const
{
	return sqrtf(x * x + y * y + z * z);
}



inline void Vector4d::normalize()
{
	float len = calculateLenOfVector();
	x /= len;
	y /= len;
	z /= len;
}