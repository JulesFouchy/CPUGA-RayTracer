
#include "Image/Image.h"
#include <Cool/Camera/Camera.h>

using namespace Cool;

#include <c3ga/c3gaTools.hpp>

int main() {
	Log::Initialize();
	auto pt = c3ga::point<double>(1., 0.5, 0.1);
	//
	Camera camera;
	Image image(200, 200);
	for (Pixel px : image) {
		Ray ray = camera.rayThroughPixel({ px.xInt(), px.yInt() });
		if (ray.direction().y > 0)
			px.color().set(0.8f, 0.1f, 0.4f);
		else
			px.color().set(0.95f, 0.7f, 0.8f);
	}
	image.saveAs("out/test.png");
}