#include "Image/Image.h"
#include <Cool/Camera/Camera.h>
#include <Cool/App/RenderState.h>
#include <c3ga/c3gaTools.hpp>

using namespace Cool;


int main() {
	// Init
	Log::Initialize();
	RenderState::SubscribeToSizeChanges([]() {Log::Info("The size is now {} {}", RenderState::Size().width(), RenderState::Size().height()); });
	// Create image
	Image image(200, 200);
	RenderState::setExportSize(image.width(), image.height());
	RenderState::setIsExporting(true);
	// Drawing
	Camera camera;
	for (Pixel px : image) {
		Ray ray = camera.rayThroughPixel({ px.xInt(), px.yInt() });
		if (ray.direction().y > 0)
			px.color().set(0.8f, 0.1f, 0.4f);
		else
			px.color().set(0.95f, 0.7f, 0.8f);
	}
	// Save
	image.saveAs("out/test.png");
}