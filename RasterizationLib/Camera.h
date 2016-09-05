#pragma once
#include "Vector4d.h"
#include "help_functions.h"
#include "Matrix4d.h"

struct Camera
{
public:
	Camera();
	Camera(Vector4d const& z_vector, float rad);
	Camera& operator=(Camera const& cam) = default;
	void CalculatePosition();
	void CaclulateOffset();
	~Camera();
	void goUp();
	void goDown();
	void goLeft();
	void goRight();
	void rotateToLeft();
	void rotateToRight();

	float coefficient_of_rotate = 0.1f;
	Vector4d radius_vector, offset_vector, z_vector;
	Matrix4d transform_matrix_from_camera_to_world_space;
	Matrix4d transform_matrix_from_world_to_camera_space;
};

