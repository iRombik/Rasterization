#pragma once
#include <Windows.h>
#include <memory>
#include "ComplexTriangle.h"
#include "Cube.h"
#include "Matrix4d.h" 
#include "LightSource.h"
#include "help_functions.h"

class Painter
{
public:
	Painter();
	Painter(HDC thdc, int w, int h);
	void DrawCube(Cube & cube, BYTE* image_buffer) const;
	void RasterizeTriangle(ComplexTriangle& triangle, Matrix4d& transform_matrix,
		Pixel* left_border_buffer, Pixel* right_border_buffer,
		BYTE* image_buffer) const;

	~Painter();

private:
	void coloringVertex(Vertex& vertex) const;
	void coloringTriangle(ComplexTriangle& triangle) const;
	void setRGBPixel(BYTE* pixel_pointer, Color const& clr) const;
	void setBufferToDevice(BYTE* image_buffer) const;
	void interpolateParameters(ComplexTriangle& triangle, float *barycentric_coordinates,
		float& result_x, float& result_y, float& result_z, Color& result_diffuse_color, Color& result_specular_color) const;

private:
	HDC hdc;
	int width, height;
};

