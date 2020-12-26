#include "App.h"

#include <Cool/App/RenderState.h>
#include <Cool/App/Input.h>

App::App()
	: m_image(100, 100), m_displayTextureShader("Cool/Renderer_Fullscreen/fullscreen.vert", "shaders/displayTexture.frag")
{
	// Register the rendering size
	RenderState::setExportSize(m_image.width(), m_image.height());
	RenderState::setIsExporting(true);
	//
	m_texture.genTexture(GL_NEAREST);
	computeImage();
}

void App::update() {
	m_camera.update();
	render();
}

void App::computeImage() {
	double sphereRadius = 2.;
	glm::vec3 sphereCenterGlm(0.f);
	auto sphereCenter = c3ga::point<double>(sphereCenterGlm.x, sphereCenterGlm.y, sphereCenterGlm.z);
	auto dualSphere = c3ga::dualSphere<double>(sphereCenterGlm.x, sphereCenterGlm.y, sphereCenterGlm.z, sphereRadius);
	auto lightDir = (
		-1. * c3ga::e2<double>()
		);
	//lightDir /= lightDir.norm();
	for (Pixel px : m_image) {
		// Get ray
		Ray ray = m_camera.rayThroughPixel({ px.xInt(), px.yInt() });
		auto rayDir = (
			ray.direction().x * c3ga::e1<double>() +
			ray.direction().y * c3ga::e2<double>() +
			ray.direction().z * c3ga::e3<double>()
			);
		auto rayPos = c3ga::point<double>(ray.origin().x, ray.origin().y, ray.origin().z);
		auto rayLine = rayPos ^ rayDir ^ c3ga::ei<double>();
		// Intersection
		auto intersectionPtPair = !(dualSphere ^ (!rayLine));
		double ptPairSQ = intersectionPtPair | intersectionPtPair;
		// Coloring
		if (ptPairSQ > 0) {// intersection
			// Normal
			c3ga::Mvec<double> p1, p2;
			c3ga::extractPairPoint(intersectionPtPair, p1, p2);
			auto normal = p2 - sphereCenter;
			normal /= normal.norm();
			//
			double t = -lightDir | normal;
			px.color().set(
				(float)t,
				(float)t,
				(float)t
			);
		}
		else {
			px.color().set(0.1f, 0.2f, 0.7f);
		}
	}
	// Put in the texture
	m_texture.uploadRGBA(m_image.width(), m_image.height(), m_image.data());
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
	if (ImGui::Button("Save")) {
		m_image.saveAs("out/test.png");
	}
	if (ImGui::Button("Compute Image")) {
		computeImage();
	}
	ImGui::Text("Camera Transform matrix :");
	glm::mat4 m = m_camera.transformMatrix();
	ImGui::Text("%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f",
		m[0][0], m[1][0], m[2][0], m[3][0],
		m[0][1], m[1][1], m[2][1], m[3][1],
		m[0][2], m[1][2], m[2][2], m[3][2],
		m[0][3], m[1][3], m[2][3], m[3][3]
	);
	ImGui::End();
}

void App::onEvent(const SDL_Event& e) {
	switch (e.type) {

	case SDL_MOUSEMOTION:
		if (!ImGui::GetIO().WantCaptureMouse) {

		}
		break;

	case SDL_MOUSEWHEEL:
		m_camera.onWheelScroll(e.wheel.y);
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (!ImGui::GetIO().WantCaptureMouse) {
			switch (e.button.button) {
			case SDL_BUTTON_LEFT:
				m_camera.onWheelDown();
				break;
			case SDL_BUTTON_RIGHT:
				break;
			case SDL_BUTTON_MIDDLE:
				m_camera.onWheelDown();
				break;
			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (!ImGui::GetIO().WantCaptureMouse) {
			switch (e.button.button) {
			case SDL_BUTTON_LEFT:
				m_camera.onWheelUp();
				break;
			case SDL_BUTTON_RIGHT:
				break;
			case SDL_BUTTON_MIDDLE:
				m_camera.onWheelUp();
				break;
			}
		}
		break;

	case SDL_KEYDOWN:
		if (!ImGui::GetIO().WantTextInput) {
			if (e.key.keysym.sym == ' ') {
				computeImage();
			}
		}
		break;

	case SDL_KEYUP:
		if (!ImGui::GetIO().WantTextInput) {

		}
		break;

	default:
		break;
	}
}