#pragma once

struct Pixel
{
	Pixel() {

	}

	Pixel(int tx, int ty) : x(tx), y(ty) 
	{
	}

	~Pixel() {

	}

	int x, y;
};

