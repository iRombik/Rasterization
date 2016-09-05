#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "Painter.h"
#include <cassert>

Painter::Painter()
{
}

Painter::Painter(HDC thdc, int w, int h): hdc(thdc), width(w), height(h)
{
}


void Painter::DrawCube(Cube & cube, Camera const& camera,
	bool rotate_flag, float material_specular_exponent, 
	Pixel* left_border_buffer, Pixel* right_border_buffer,
	BYTE* image_buffer) const
{
	Matrix4d rotate = Matrix4d::rotateAboutVector(
		Vector4d(0, cosf(0.6f), sinf(0.6f), 0), // vector around which rotates
		float(1.0f / 180.0f * M_PI));			  // the angle of rotation
	Triangle cube_triangles[12];
	
	if (rotate_flag) {
		cube.rotateCube(rotate);
	}
	cube.giveTriangles(cube_triangles);

	Matrix4d projection_to_clip_space = Matrix4d::perspectiveProjectionMatrix(1, 1, -1, -1000);
	Matrix4d projection_to_screen = Matrix4d::projectionToScreen(float(width), float(height));
	Matrix4d result_transform_matrix = projection_to_screen * projection_to_clip_space ;

	LightSource ls(camera.transform_matrix_from_world_to_camera_space * Vector4d(0, 0, 1000000, 1),
		Color(1, 1, 1), 1, 0, 0);

	for (int i = 0; i < 12; ++i) {
		cube_triangles[i].TransformTriangle(camera.transform_matrix_from_world_to_camera_space);
		ComplexTriangle triangle_for_rasterization = cube_triangles[i];
		RasterizeTriangle(triangle_for_rasterization, result_transform_matrix,
			ls, material_specular_exponent,
			left_border_buffer, right_border_buffer, image_buffer);
	}

	setBufferToDevice(image_buffer);
}


void Painter::RasterizeTriangle(ComplexTriangle& triangle, Matrix4d& transform_matrix,
	LightSource const& ls, float material_specular_exponent,
	Pixel* left_border_buffer, Pixel* right_border_buffer,
	BYTE* image_buffer) const
{
	
	Color ambient(0.2f, 0.2f, 0.2f);
	
	triangle.CalculateNormalVector();
	triangle.normal_vector.normalize();

	for (int i = 0; i < 3; ++i) {
		triangle.complex_vertices[i].normal_vector = triangle.normal_vector;
	
		//calculate projection coordinate
		triangle.complex_vertices[i].multCoordinate(transform_matrix);
		triangle.complex_vertices[i].persrectiveDivide();
	}
	coloringTriangle(triangle);

	float triangle_area = help_functions::calculateArea(
		triangle.complex_vertices[0].position,
		triangle.complex_vertices[1].position,
		triangle.complex_vertices[2].position
	);

	//calculate border pixels for triangle 
	Triangle temporary_simple_triangle_in_screen_space;
	for (int i = 0; i < 3; ++i) {
		temporary_simple_triangle_in_screen_space.vertices[i] = triangle.complex_vertices[i].position;
	}
	temporary_simple_triangle_in_screen_space.CalculateNormalVector();
	Vector4d z(0, 0, 1, 0);

	if (help_functions::dotProduct(temporary_simple_triangle_in_screen_space.normal_vector, z) > 0.0f) {
		return;
	}

	help_functions::sortByYCoordinate(
		temporary_simple_triangle_in_screen_space.vertices[0],
		temporary_simple_triangle_in_screen_space.vertices[1],
		temporary_simple_triangle_in_screen_space.vertices[2]
	);

	int triangle_height = int(
		temporary_simple_triangle_in_screen_space.vertices[2].y - temporary_simple_triangle_in_screen_space.vertices[0].y
		) + 1;
	help_functions::getBorderPixelsOfTriangle(left_border_buffer, right_border_buffer, temporary_simple_triangle_in_screen_space);

	//main cycle
	for (int i = 0; i < triangle_height; ++i) {
		assert(right_border_buffer[i].y == left_border_buffer[i].y);
		int pos_y = right_border_buffer[i].y;
		//assert(pos_y > 0 && pos_y < height);
		for (int pos_x = left_border_buffer[i].x; pos_x <= right_border_buffer[i].x; ++pos_x) {
			//assert(pos_x > 0 && pos_x < width);
			Vector4d vector_to_point(float(pos_x), float(pos_y), 0, 1);
			float barycentric_coordinates[3];
			triangle.calculateBarycenticCoodinates(vector_to_point, triangle_area, barycentric_coordinates);

			//calculate vertex parameters
			float vertex_x, vertex_y, vertex_z;
			Color vertex_diffuse_color, vertex_specular_color;
			interpolateParameters(triangle, barycentric_coordinates,
				vertex_x, vertex_y, vertex_z, vertex_diffuse_color, vertex_specular_color);	

			Vector4d vertex_position(vertex_x, vertex_y, vertex_z, 1);
			Vector4d vertex_normal = (triangle.complex_vertices[0].normal_vector +
				triangle.complex_vertices[1].normal_vector +
				triangle.complex_vertices[2].normal_vector);
			vertex_normal.normalize();

			//calculate pixel color with Phong shading
			Color diffuse_term, specular_term;
			ls.calculateShading(vertex_position, vertex_normal, material_specular_exponent, diffuse_term, specular_term);
			Color pixel_color = (diffuse_term + ambient) * vertex_diffuse_color + specular_term * vertex_specular_color;
			pixel_color.getSafeColor();

			//set pixel color to buffer
			int index_in_buffer = (int(pos_x) + int(pos_y) * int(width)) * 4;
			setRGBPixel(&image_buffer[index_in_buffer], pixel_color);
		}
	}
}

void Painter::coloringVertex(Vertex & vertex) const
{
	vertex.diffuse_color = Color();
	vertex.specular_color = Color(1, 1, 1);
}

void Painter::coloringTriangle(ComplexTriangle & triangle) const
{
	for (int i = 0; i < 3; ++i) {
		coloringVertex(triangle.complex_vertices[i]);
	}
}

void Painter::setRGBPixel(BYTE * pixel_pointer, Color const& pixel_color) const
{
	*(pixel_pointer + 2) = BYTE(pixel_color.R * 255.0);
	*(pixel_pointer + 1) = BYTE(pixel_color.G * 255.0);
	*(pixel_pointer + 0) = BYTE(pixel_color.B * 255.0);
}

void Painter::setBufferToDevice(BYTE * image_buffer) const
{
	tagBITMAPINFO bif;
	bif.bmiHeader.biSize = sizeof(bif.bmiHeader);
	bif.bmiHeader.biWidth = width; //ширина
	bif.bmiHeader.biHeight = -height; //выста (если взять положительную, то рисунок будет перевурнут)
	bif.bmiHeader.biSizeImage = 0;
	bif.bmiHeader.biBitCount = 32;//Глубина цвета
	bif.bmiHeader.biPlanes = 1;
	bif.bmiHeader.biCompression = BI_RGB;

	SetDIBitsToDevice(hdc,
		0,
		0,
		width,
		height,
		0,
		0,
		0,
		height,
		image_buffer,
		&bif,
		DIB_RGB_COLORS);
}

void Painter::interpolateParameters(ComplexTriangle & triangle, float * barycentric_coordinates,
	float & result_x, float & result_y, float & result_z, Color& result_diffuse_color, Color& result_specular_color) const
{
	float reciprocal_z[3] = { 1.0f / triangle.vertices[0].z,
		1.0f / triangle.vertices[1].z,
		1.0f / triangle.vertices[2].z } ;

	result_x = triangle.vertices[0].x *  barycentric_coordinates[0] +
		triangle.vertices[1].x *  barycentric_coordinates[1] +
		triangle.vertices[2].x *  barycentric_coordinates[2];

	result_y = triangle.vertices[0].y *  barycentric_coordinates[0] + 
		triangle.vertices[1].y *  barycentric_coordinates[1] +
		triangle.vertices[2].y *  barycentric_coordinates[2];

	result_z = 1.0f / (reciprocal_z[0] *  barycentric_coordinates[0] +
		reciprocal_z[1] * barycentric_coordinates[1] + 
		reciprocal_z[2] * barycentric_coordinates[2]);
	/*
	result_diffuse_color = (triangle.complex_vertices[0].diffuse_color * barycentric_coordinates[0] * reciprocal_z[0] +
		triangle.complex_vertices[1].diffuse_color * barycentric_coordinates[1] * reciprocal_z[1] +
		triangle.complex_vertices[2].diffuse_color * barycentric_coordinates[2] * reciprocal_z[2]) * result_z;
	result_diffuse_color.getSafeColor();

	result_specular_color = (triangle.complex_vertices[0].specular_color * barycentric_coordinates[0] * reciprocal_z[0] +
		triangle.complex_vertices[1].specular_color * barycentric_coordinates[1] * reciprocal_z[1] +
		triangle.complex_vertices[2].specular_color * barycentric_coordinates[2] * reciprocal_z[2]) * result_z;
	result_specular_color.getSafeColor();
	*/
	result_diffuse_color = Color(0.5, 0.5, 0.5);
	result_specular_color = Color(1, 1, 1);
}





Painter::~Painter()
{
}

