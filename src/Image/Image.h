#pragma once

#include "ImageIterator.h"
#include "Color.h"

/// Wrapper for a vector of Colors (4 unsigned chars) representing an image
class Image {
	friend class ImageIterator;
public:
	Image(unsigned int width, unsigned int height);
	~Image() = default;

	void saveAs(const char* filepath);

	inline unsigned int width()  { return m_width; }
	inline unsigned int height() { return m_height; }
	/// width / height
	inline float aspectRatio() { return m_aspectRatio; }
	inline Color& colorAt(unsigned int x, unsigned int y) { return m_pixelColors[x + y * m_width]; }

	ImageIterator begin();
	ImageIterator end();

private:
	unsigned int m_width;
	unsigned int m_height;
	float m_aspectRatio;
	/// Follows OpenGL's convention : (0, 0) is the bottom-left of the image
	/// Pixels are ordered by row first (As you read through the vector x increases up to width-1 (included), then wraps back to 0 and y goes up by 1, and so on)
	std::vector<Color> m_pixelColors;
};