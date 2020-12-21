
#include "Image/Image.h"

using namespace Cool;

#include <c3ga/c3gaTools.hpp>

int main() {
	auto pt = c3ga::point<double>(1., 0.5, 0.1);
	//
	Image image(1280, 720);
	for (Pixel px : image) {
		if (abs(px.x()) + abs(px.y()) < 0.7f)
			px.color().set(0.8f, 0.1f, 0.4f);
		else
			px.color().set(0.95f, 0.7f, 0.8f);
	}
	image.saveAs("out/test.png");
}