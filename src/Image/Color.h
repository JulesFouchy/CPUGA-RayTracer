#pragma once

/// Color stored as 4 unsigned chars representing Red, Green, Blue and Alpha channels
class Color {
public:
	Color();
	~Color() = default;

	/// Takes values in the range [0, 255] (values outside will wrap)
	void set(unsigned char r, unsigned char g, unsigned char b);
	/// Takes values in the range [0, 1] (values outside are clamped to 0 or 1)
	void set(float r, float g, float b);

private:
	unsigned char m_channels[4];
};