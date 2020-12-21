#include "Image.h"

//#include <stb_image/stb_image_write.h>

Image::Image(unsigned int width, unsigned int height)
	: m_width(width), m_height(height), m_aspectRatio(static_cast<float>(width) / static_cast<float>(height))
{
	m_pixelColors.resize(m_width * m_height);
}

void Image::saveAs(const char* filepath) {
	//stbi_flip_vertically_on_write(1);
	//stbi_write_png(filepath, m_width, m_height, 4, m_pixelColors.data(), 0);
}

ImageIterator Image::begin() {
	return ImageIterator(*this, 0, 0);
}

ImageIterator Image::end() {
	return ImageIterator(*this, 0, m_height);
}