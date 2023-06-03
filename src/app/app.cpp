#include "app.h"
#include "log/log.h"

#include <exception>
#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace SIGame
{
/* Init basic application settings */
bool App::Init()
{
	/* Init OpenGL and ImGui stuff */
	if(!glfwInit())
	{
		LOG_ERROR("Couldn't initialize GLFW");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* FIXME: Maybe make fullscreen only? If not, then add a windowed option */
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// GLFWmonitor* primary = glfwGetPrimaryMonitor();
	// const GLFWvidmode* mode = glfwGetVideoMode(primary);

	/* TODO: Make a file with settings and get window width and height from there */
	/* TODO: Maybe make funny titles like in terraria and minecraft? :) */
	/* TODO: For borderless fullscreen see https://github.com/glfw/glfw/issues/1036#issuecomment-311091384 */
	m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, "SIGame", nullptr, nullptr);
	// glfwSetWindowMonitor(m_Window, primary, 0, 0, mode->width, mode->height, mode->refreshRate);
	if(m_Window == nullptr)
	{
		LOG_ERROR("Failed to create window");
		return false;
	}

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1); // Enable VSYNC

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Init imgui platform backends
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// glfwSetKeyCallback(m_Window, gl_key_callback);
	// glfwSetWindowSizeCallback(m_Window, gl_window_size_callback);

	while(!glfwWindowShouldClose(m_Window)) {
		glfwPollEvents();
		// process_keys();

		// Start Imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Render end
		ImGui::Render();
		glViewport(0, 0, m_WindowWidth, m_WindowHeight);
		glClearColor(0.5, 0.33, 0.7, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(m_Window);
	}

	return true;
}

void App::Run()
{

}
} /* namespace SIGame */