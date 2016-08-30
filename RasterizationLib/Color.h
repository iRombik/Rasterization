#pragma once

struct Color
{
	Color(float r = 0.5, float g = 0.5, float b = 0.5) : R(r), G(g), B(b)
	{

	}

	Color Color::operator*(float mult) const
	{
		return Color(mult * R, mult * G, mult * B);
	}

	Color Color::operator*(Color const& clr) const
	{
		return Color(R * clr.R, G * clr.G, B * clr.B);
	}

	Color Color::operator+(Color const & clr) const
	{
		return Color(R + clr.R, G + clr.G, B + clr.B);
	}

	Color & Color::operator+=(Color const & clr)
	{
		R += clr.R;
		G += clr.G;
		B += clr.B;
		return *this;
	}

	void getSafeColor()
	{
		R = R < 1.0f ? R : 1.0f;
		G = G < 1.0f ? G : 1.0f;
		B = B < 1.0f ? B : 1.0f;
	}

	~Color()
	{
	}

	float R, G, B;
};













