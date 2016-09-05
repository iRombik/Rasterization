#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
}

Camera::Camera(Vector4d const & z_vector, float rad):
	z_vector(z_vector),  radius_vector(0, 0, rad, 0)
{
}

void Camera::CalculatePosition()
{
	Vector4d y_vector(0, 1, 0, 0);
	Vector4d x_vector(help_functions::crossProduct(y_vector, z_vector));
	y_vector = help_functions::crossProduct(z_vector, x_vector);

	x_vector.normalize();
	y_vector.normalize();
	z_vector.normalize();
	float center_coordinate[4][4]{
		{ x_vector.x, x_vector.y, x_vector.z, 0 },
		{ y_vector.x, y_vector.y, y_vector.z, 0 },
		{ z_vector.x, z_vector.y, z_vector.z, 0 },
		{ 0, 0, 0, 1 }
	};
	transform_matrix_from_camera_to_world_space = center_coordinate;
	CaclulateOffset();
	transform_matrix_from_camera_to_world_space.SetDisplacementVector(offset_vector);
	transform_matrix_from_world_to_camera_space = transform_matrix_from_camera_to_world_space.FindInverseMatrix();
}

void Camera::goUp()
{
	static Matrix4d rotate_matrix = Matrix4d::rotateAboutX(-coefficient_of_rotate);
	transform_matrix_from_camera_to_world_space = transform_matrix_from_camera_to_world_space * rotate_matrix;
	CaclulateOffset();
	transform_matrix_from_camera_to_world_space.SetDisplacementVector(offset_vector);
	transform_matrix_from_world_to_camera_space = transform_matrix_from_camera_to_world_space.FindInverseMatrix();
}

void Camera::goDown()
{
	static Matrix4d rotate_matrix = Matrix4d::rotateAboutX(coefficient_of_rotate);
	transform_matrix_from_camera_to_world_space = transform_matrix_from_camera_to_world_space * rotate_matrix;
	CaclulateOffset();
	transform_matrix_from_camera_to_world_space.SetDisplacementVector(offset_vector);
	transform_matrix_from_world_to_camera_space = transform_matrix_from_camera_to_world_space.FindInverseMatrix();
}

void Camera::goLeft()
{
	static Matrix4d rotate_matrix = Matrix4d::rotateAboutY(coefficient_of_rotate);
	transform_matrix_from_camera_to_world_space = transform_matrix_from_camera_to_world_space * rotate_matrix;
	CaclulateOffset();
	transform_matrix_from_camera_to_world_space.SetDisplacementVector(offset_vector);
	transform_matrix_from_world_to_camera_space = transform_matrix_from_camera_to_world_space.FindInverseMatrix();
}

void Camera::goRight()
{
	static Matrix4d rotate_matrix = Matrix4d::rotateAboutY(-coefficient_of_rotate);
	transform_matrix_from_camera_to_world_space = transform_matrix_from_camera_to_world_space * rotate_matrix;
	CaclulateOffset();
	transform_matrix_from_camera_to_world_space.SetDisplacementVector(offset_vector);
	transform_matrix_from_world_to_camera_space = transform_matrix_from_camera_to_world_space.FindInverseMatrix();
}

void Camera::rotateToLeft()
{
	static Matrix4d rotate_matrix = Matrix4d::rotateAboutZ(-coefficient_of_rotate);
	transform_matrix_from_camera_to_world_space = transform_matrix_from_camera_to_world_space * rotate_matrix;
	transform_matrix_from_world_to_camera_space = transform_matrix_from_camera_to_world_space.FindInverseMatrix();
}

void Camera::rotateToRight()
{
	static Matrix4d rotate_matrix = Matrix4d::rotateAboutZ(coefficient_of_rotate);
	transform_matrix_from_camera_to_world_space = transform_matrix_from_camera_to_world_space * rotate_matrix;
	transform_matrix_from_world_to_camera_space = transform_matrix_from_camera_to_world_space.FindInverseMatrix();
}


void Camera::CaclulateOffset()
{
	offset_vector = transform_matrix_from_camera_to_world_space * radius_vector;
}


Camera::~Camera()
{
}

