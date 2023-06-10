#pragma once
#include "common.h"

#include <GLFW/glfw3.h>

namespace SIGame
{
/*
* FIXME: For now, window class is fully static. It is supposed, that
* only one window would be present. Maybe, in the future this might
* require a rewrite.
*/
class Window
{
	public:
		/* Delete all constructors */
		Window()                   = delete;
		Window(Window&&)           = delete;
		Window(const Window&)      = delete;
		Window operator=(Window&)  = delete;
		~Window() = delete;

		static bool Init(int width, int height);
		static bool DeInit();

		static GLFWwindow* GetWindow();
		static int GetWidth();
		static int GetHeight();

	private:
		static void GL_WindowSizeCallback(GLFWwindow* window, int new_width, int new_height);

	private:
		static inline int m_WindowWidth = 0;
		static inline int m_WindowHeight = 0;
		static inline GLFWwindow* m_Window = nullptr;

};
} /* namespace SIGame */