#include "stdafx.h"
#include "help_functions.h"
#include "Triangle.h"


namespace help_functions {

	void sortByYCoordinate(Vector4d & a, Vector4d & b, Vector4d & c)
	{
		if (a.y > b.y) {
			std::swap(a, b);
		}
		if(b.y > c.y){
			std::swap(b, c);
		}
		if (a.y > b.y) {
			std::swap(a, b);
		}
	}

	Vector4d getPointOnTheSegmentCorrespondingToGiven(
		Vector4d const & given_point,
		Vector4d const & first_point_of_segment, Vector4d const & second_point_of_segment
	)
	{
		float dx = second_point_of_segment.x - first_point_of_segment.x;
		float dy = second_point_of_segment.y - first_point_of_segment.y;
		float d = dx / dy;
		float reqired_y_difference = given_point.y - first_point_of_segment.y;
		return Vector4d(first_point_of_segment.x + d * reqired_y_difference, given_point.y, 0, 1);
	}

	Pixel* rasterizeLine(
		Vector4d const & first_point_of_segment,
		Vector4d const & second_point_of_segment,
		Pixel * pixels_belonging_to_line)
	{
		float dx = second_point_of_segment.x - first_point_of_segment.x;
		float dy = second_point_of_segment.y - first_point_of_segment.y;
		if (dy < 1e-5) {
			return ++pixels_belonging_to_line;
		}
		float d = dx / dy;
		int div_d = int(d);
		float trash, mod_d = std::modf(std::fabs(d), &trash);
		int complement =  dx > 0 ? 1 : -1;
		float deviation = mod_d - 0.5f;
		int x = int(first_point_of_segment.x);
		int y = int(first_point_of_segment.y);

		*(pixels_belonging_to_line++) = Pixel(x, y);
		while (++y <= int(second_point_of_segment.y)) {
			if (deviation >= 0) {
				--deviation;
				x += complement;
			}
			x += div_d;
			deviation += mod_d;
			*(pixels_belonging_to_line++) = Pixel(x, y);
		}

		return pixels_belonging_to_line;
	}

	void getBorderPixelsOfTriangle(Pixel * left_border, Pixel * right_border, Triangle & triangle)
	{
		Pixel *simple_border, *complex_border;
		float area = calculateArea(triangle.vertices[0], triangle.vertices[1],
			triangle.vertices[2]);
		if(area >= 0){
			simple_border = left_border;
			complex_border = right_border;
		}
		else {
			simple_border = right_border;
			complex_border = left_border;
		}

		rasterizeLine(triangle.vertices[0], triangle.vertices[2], simple_border);
		complex_border = rasterizeLine(triangle.vertices[0], triangle.vertices[1], complex_border);
		rasterizeLine(triangle.vertices[1], triangle.vertices[2], complex_border - 1);
	}
}//help_funtions
