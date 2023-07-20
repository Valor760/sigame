#include "app.h"
#include "window.h"
#include "core/layout.h"

#include <exception>
#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace SIGame::App
{
BUTTON_CALLBACK_FUNC(SwitchLayout)
{
	if(args.size() != 1)
	{
		LOG_ERROR("Invalid number of arguments: %d received (1 expected)", args.size());
		return;
	}

	const std::string& layout_name = args[0];
	LOG_DEBUG("Switching to layout - \'%s\'", layout_name.c_str());
	if(!Core::LayoutManager::LoadLayout(layout_name))
	{
		LOG_ERROR("Failed to switch layout");
	}
}

/* Init basic application settings */
bool MainApp::Init()
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

	/* Create temporary directory */
	if(fs::exists(TMP_DIR))
	{
		fs::remove_all(TMP_DIR);
	}
	/* This already creates tmp dir */
	fs::create_directories(SIQ_EXTRACT_DIR);

	Core::ADD_BUTTON_CALLBACK(SwitchLayout);

	/* Do this AT THE END, so every component had time to add it's callbacks */
	Core::LayoutManager::LoadLayout("Main Menu");

	return true;
}

void MainApp::Run()
{
	glClearColor(0, 0, 0, 1.0);

	while(!glfwWindowShouldClose(Window::GetWindow())) {
		glfwWaitEvents();
		glfwPollEvents();

		// Start Imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glViewport(0, 0, Window::GetWidth(), Window::GetHeight());

		Core::LayoutManager::DrawLayout();

		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();

		// Render end
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(Window::GetWindow());
	}
}

MainApp::~MainApp()
{
	/* Remove tmp dir */
	fs::remove_all(TMP_DIR);

	Window::DeInit();
}
} /* namespace SIGame */
