#include "window.h"
#include "log/log.h"

namespace SIGame::App
{
void GL_WindowSizeCallback(GLFWwindow* window, int new_width, int new_height)
{
	ImVec2 prev_size = Window::GetSize();
	LOG_DEBUG("Window size changed: [prev %dx%d] [new %dx%d]", (int)prev_size.x, (int)prev_size.y, new_width, new_height);

	Window::SetSize(ImVec2(new_width, new_height));
}

bool Window::InitImpl(int width, int height)
{
	if(width <= 0 || height <= 0)
	{
		LOG_ERROR("Wrong window width or height provided");
		return false;
	}
	/*
		TODO: Get screen resolution. If width and height provided are greater than screen res,
		then crop to the screen res.
	*/
	
	m_WindowWidth = width;
	m_WindowHeight = height;

	if(!glfwInit())
	{
		LOG_ERROR("Can't initialize GLFW");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* FIXME: Maybe make fullscreen only? If not, then add a windowed option */
	/* TODO: Remove resize completely, allow rezise only using settings */
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// GLFWmonitor* primary = glfwGetPrimaryMonitor();
	// const GLFWvidmode* mode = glfwGetVideoMode(primary);

	/* TODO: Make a file with settings and get window width and height from there */
	/* TODO: Maybe make funny titles like in terraria and minecraft? :) */
	/* TODO: For borderless fullscreen see https://github.com/glfw/glfw/issues/1036#issuecomment-311091384 */
	m_Window = glfwCreateWindow(width, height, "SIGame", nullptr, nullptr);
	// glfwSetWindowMonitor(m_Window, primary, 0, 0, mode->width, mode->height, mode->refreshRate);
	if(m_Window == nullptr)
	{
		LOG_ERROR("Failed to create window");
		return false;
	}

	glfwMakeContextCurrent(m_Window);
	/* TODO: Add an option to customize this setting */
	glfwSwapInterval(1); // Enable VSYNC

	glfwSetWindowSizeCallback(m_Window, GL_WindowSizeCallback);

	return true;
}

bool Window::DeInitImpl()
{
	if(m_Window)
	{
		glfwDestroyWindow(m_Window);
	}

	return true;
}

GLFWwindow* Window::GetWindow()
{
	return Get().m_Window;
}

int Window::GetWidth()
{
	return Get().m_WindowWidth;
}

int Window::GetHeight()
{
	return Get().m_WindowHeight;
}

ImVec2 Window::GetSize()
{
	Window& instance = Get();
	return ImVec2(instance.m_WindowWidth, instance.m_WindowHeight);
}

void Window::SetSize(ImVec2 new_size)
{
	Window& instance = Get();
	instance.m_WindowWidth = (int)new_size.x;
	instance.m_WindowHeight = (int)new_size.y;
}
} /* namespace SIGame */
