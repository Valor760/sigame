#include "app.h"
#include "log/log.h"
#include "window.h"

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
	/* FIXME: Read window width and height from some settings file */
	if(!Window::Init(1600, 900))
	{
		LOG_ERROR("Window init failed!");
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Init imgui platform backends
	ImGui_ImplGlfw_InitForOpenGL(Window::GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");

	return true;
}

void App::Run()
{
	while(!glfwWindowShouldClose(Window::GetWindow())) {
		glfwPollEvents();

		// Start Imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		// Render end
		ImGui::Render();
		glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
		glClearColor(0.5, 0.33, 0.7, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(Window::GetWindow());
	}

}
} /* namespace SIGame */