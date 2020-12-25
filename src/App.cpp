#include "App.h"

#include <Cool/App/RenderState.h>

App::App()
	: m_image(100, 100), m_displayTextureShader("Cool/Renderer_Fullscreen/fullscreen.vert", "shaders/displayTexture.frag")
{
	// Register the rendering size
	RenderState::setExportSize(m_image.width(), m_image.height());
	RenderState::setIsExporting(true);
	// Drawing
	auto dualSphere = c3ga::dualSphere<double>(0., 0., 0., 1);
	for (Pixel px : m_image) {
		// Get ray
		Ray ray = m_camera.rayThroughPixel({ px.xInt(), px.yInt() });
		auto dir = (
			ray.direction().x * c3ga::e1<double>() +
			ray.direction().y * c3ga::e2<double>() +
			ray.direction().z * c3ga::e3<double>()
		);
		auto pos = c3ga::point<double>(ray.origin().x, ray.origin().y, ray.origin().z);
		auto rayLine = pos ^ dir ^ c3ga::ei<double>();
		// Intersection
		auto intersectionPtPair = !(dualSphere ^ (!rayLine));
		// Coloring
		double square = intersectionPtPair | intersectionPtPair;
		px.color().set(square * 0.4 + 0.5, 0.2f, square > 0 ? 1.f : 0.f);
	}
	// Put in the texture
	m_texture.genTexture(GL_NEAREST);
	m_texture.uploadRGBA(m_image.width(), m_image.height(), m_image.data());
	// Save
	//m_image.saveAs("out/test.png");
}

void App::update() {
	render();
}

void App::render() {
	m_renderer.begin();
		m_texture.bindToSlot(0);
		m_displayTextureShader.bind();
		m_displayTextureShader.setUniform1i("u_TextureSlot", 0);
		m_renderer.render();
	m_renderer.end();
}

void App::ImGuiWindows() {
	ImGui::Begin("Hello");
	ImGui::End();
}