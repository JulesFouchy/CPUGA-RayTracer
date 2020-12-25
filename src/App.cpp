#include "App.h"

#include <Cool/App/RenderState.h>
#include <c3ga/c3gaTools.hpp>

App::App()
	: m_image(200, 200), m_displayTextureShader("Cool/Renderer_Fullscreen/fullscreen.vert", "shaders/displayTexture.frag")
{
	// Register the rendering size
	RenderState::setExportSize(m_image.width(), m_image.height());
	RenderState::setIsExporting(true);
	// Drawing
	for (Pixel px : m_image) {
		Ray ray = m_camera.rayThroughPixel({ px.xInt(), px.yInt() });
		if (ray.direction().z > 0)
			px.color().set(0.8f, 0.1f, 0.4f);
		else
			px.color().set(0.95f, 0.7f, 0.8f);
	}
	// Put in the texture
	m_texture.genTexture();
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