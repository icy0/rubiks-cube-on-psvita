#pragma once

// glfw headers
#include <GLFW/glfw3.h>

namespace Rubiks_Cube_Game
{
class GLFW_Library
{
private:
	GLFWwindow* window;
	int m_framebuffer_width;
	int m_framebuffer_height;

public:
	GLFW_Library();
	~GLFW_Library();
	GLFWwindow& get_window() { return *window; }
	bool window_should_close();
};
}