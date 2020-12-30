#include "App.h"

#include <Cool/App/RenderState.h>
#include <Cool/App/Input.h>

#include <chrono>

#define IMG_SIZE 100

App::App()
	: m_imageGA(IMG_SIZE, IMG_SIZE), m_imageLA(IMG_SIZE, IMG_SIZE), m_displayTextureShader("Cool/Renderer_Fullscreen/fullscreen.vert", "shaders/displayTexture.frag")
{
	// Register the rendering size
	RenderState::setExportSize(m_imageGA.width(), m_imageGA.height());
	RenderState::setIsExporting(true);
	//
	m_textureGA.genTexture(GL_NEAREST);
	m_textureLA.genTexture(GL_NEAREST);

	// Compute images
	std::chrono::time_point<std::chrono::system_clock> startGA, endGA, startLA, endLA;
	startLA = std::chrono::system_clock::now();
	computeImageLA();
	endLA = std::chrono::system_clock::now();
	startGA = std::chrono::system_clock::now();
	computeImageGA();
	endGA = std::chrono::system_clock::now();

	// Benchmark
	std::chrono::duration<double> elapsed_secondsGA = endGA - startGA;
	std::chrono::duration<double> elapsed_secondsLA = endLA - startLA;
	Log::Release::Info("GA took {}s", elapsed_secondsGA.count());
	Log::Release::Info("LA took {}s", elapsed_secondsLA.count());
}

void App::update() {
	m_camera.update();
	render();
}

void App::computeImageGA() {
	float sphereRadius = 2.5f;
	auto sphereCenter = c3ga::point<float>(0, 0, 0);
	auto dualSphere = c3ga::dualSphere<float>(0, 0, 0, sphereRadius * sphereRadius);
	auto lightDir = (
		-1. * c3ga::e2<float>()
		);
	//lightDir /= lightDir.norm();
	for (Pixel px : m_imageGA) {
		std::chrono::time_point<std::chrono::system_clock> startConv, endConv, startCalc, endCalc;
		startConv = std::chrono::system_clock::now();
		// Get ray
		Ray ray = m_camera.rayThroughPixel({ px.xInt(), px.yInt() });
		auto rayDir = (
			ray.direction().x * c3ga::e1<float>() +
			ray.direction().y * c3ga::e2<float>() +
			ray.direction().z * c3ga::e3<float>()
			);
		auto rayPos = c3ga::point<float>(ray.origin().x, ray.origin().y, ray.origin().z);
		auto rayLine = rayPos ^ rayDir ^ c3ga::ei<float>();
		//
		endConv = std::chrono::system_clock::now();
		startCalc = std::chrono::system_clock::now();
		// Intersection
		auto intersectionPtPair = !(dualSphere ^ (!rayLine));
		double ptPairSQ = intersectionPtPair | intersectionPtPair;
		// Coloring
		if (ptPairSQ > 0) {// intersection
			// Normal
			c3ga::Mvec<float> p1, p2;
			c3ga::extractPairPoint(intersectionPtPair, p1, p2);
			auto normal = p2 - sphereCenter;
			normal /= normal.norm();
			//
			float t = -lightDir | normal;
			px.color().set(t, t, t);
		}
		else {
			px.color().set(0.1f, 0.2f, 0.7f);
		}
		endCalc = std::chrono::system_clock::now();
		//Log::Release::Info("Conversion  took {}s", (endConv - startConv).count());
		//Log::Release::Info("Calculation took {}s", (endCalc - startCalc).count());
	}
	// Put in the texture
	m_textureGA.uploadRGBA(m_imageGA.width(), m_imageGA.height(), m_imageGA.data());
}

void App::computeImageLA() {
	float sphereRadius = 2.5f;
	glm::vec3 sphereCenter(0.f);
	glm::vec3 lightDir = glm::vec3(0, -1, 0);
	for (Pixel px : m_imageLA) {
		// Get ray
		Ray ray = m_camera.rayThroughPixel({ px.xInt(), px.yInt() });
		//
		float a = glm::dot(ray.direction(), ray.direction());
		float b = 2.f * glm::dot(ray.direction(), ray.origin() - sphereCenter);
		float c = glm::dot(ray.origin() - sphereCenter, ray.origin() - sphereCenter) - sphereRadius * sphereRadius;
		float delta = b * b - 4 * a * c;
		if (delta > 0) {
			float t1 = (-b - sqrt(delta)) / 2. / a;
			float t2 = (-b + sqrt(delta)) / 2. / a;
			float tNear, tFar;
			if (t1 < t2 && t1 > 0) {
				tNear = t1;
				tFar = t2;
			}
			else {
				tNear = t2;
				tFar = t1;
			}
			glm::vec3 pNear = ray.origin() + tNear * ray.direction();
			glm::vec3 pFar = ray.origin() + tFar * ray.direction();
			glm::vec3 normal = glm::normalize(pNear - sphereCenter);
			float t = -glm::dot(lightDir, normal);
			px.color().set(t, t, t);
		}
		else {
			px.color().set(0.1f, 0.2f, 0.7f);
		}
	}
	// Put in the texture
	m_textureLA.uploadRGBA(m_imageLA.width(), m_imageLA.height(), m_imageLA.data());
}

void App::render() {
	m_renderer.begin();
		m_textureGA.bindToSlot(0);
		m_textureLA.bindToSlot(1);
		m_displayTextureShader.bind();
		m_displayTextureShader.setUniform1i("u_TextureSlotGA", 0);
		m_displayTextureShader.setUniform1i("u_TextureSlotLA", 1);
		m_renderer.render();
	m_renderer.end();
}

void App::ImGuiWindows() {
	ImGui::Begin("Hello");
	if (ImGui::Button("Save")) {
		m_imageGA.saveAs("out/GA.png");
		m_imageLA.saveAs("out/LA.png");
	}
	if (ImGui::Button("Compute Image")) {
		computeImageGA();
		computeImageLA();
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
				computeImageGA();
				computeImageLA();
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