#pragma once
#include <stdint.h>
#include <memory>

#include <GLFW/glfw3.h>

namespace SIGame
{
class App
{
	public:
		bool Init();
		void Run();
	
	private:
		GLFWwindow* m_Window = nullptr;

		/* FIXME: Read window width and height from some settings file */
		uint32_t m_WindowWidth = 1600;
		uint32_t m_WindowHeight = 900;
};
} /* namespace SIGame */