#pragma once
#include "common.h"

#include <GLFW/glfw3.h>

namespace SIGame::App
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
		Window(Window&&)           = delete;
		Window(const Window&)      = delete;

		static inline Window& Get()
		{
			static Window instance;
			return instance;
		}

		static inline bool Init(int width, int height)
		{
			return Get().InitImpl(width, height);
		}
		static inline bool DeInit()
		{
			return Get().DeInitImpl();
		}

		static GLFWwindow* GetWindow();
		static int GetWidth();
		static int GetHeight();

		static ImVec2 GetSize();
		static void SetSize(ImVec2 new_size);

	private:
		Window() {}

		bool InitImpl(int width, int height);
		bool DeInitImpl();

	private:
		static Window m_WindowInstace;

		int m_WindowWidth = 0;
		int m_WindowHeight = 0;
		GLFWwindow* m_Window = nullptr;

};
} /* namespace SIGame */