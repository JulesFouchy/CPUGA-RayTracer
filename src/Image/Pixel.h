#pragma once

#include "Color.h"
class Image;

/// The position and color of a given pixel of a given image.
/// No need to take this class by reference : it already holds a reference to the image and can modify it.
class Pixel {
public:
	Pixel(Image& image, unsigned int x, unsigned int y);
	~Pixel() = default;
	
	/// A reference to the image's color at this pixel
	Color& color();
	/// In the range [-aspectRatio, +aspectRatio]
	/// Axis pointing right
	float x();
	/// In the range [-1, 1]
	/// Axis pointing up
	float y();

private:
	Image& m_image;
	unsigned int m_x;
	unsigned int m_y;
};