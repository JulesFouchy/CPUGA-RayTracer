#pragma once

#include <Cool/App/IApp.h>

#include <Cool/Image/Image.h>
#include <Cool/Camera/Camera.h>
#include <Cool/OpenGL/Texture.h>
#include <Cool/OpenGL/Shader.h>
#include <Cool/Renderer_Fullscreen/Renderer_Fullscreen.h>

using namespace Cool;

class App : public IApp {
public:
	App();
	~App() = default;

	void update() override;
	void ImGuiWindows() override;
	void onEvent(const SDL_Event& e) override;

private:
	void render();
	void computeImage();

private:
	Image m_image;
	Camera m_camera;
	Texture m_texture;
	Shader m_displayTextureShader;
	Renderer_Fullscreen m_renderer;
};