// glfw headers
#include <GLFW/glfw3.h>

// cstdlib headers
#include <iostream>

// project internal headers
#include <GLFW_Library.h>
#include <Logging.h>
#include <Global.h>

namespace Rubiks_Cube_Game
{
bool GLFW_Library::window_should_close()
{
	return glfwWindowShouldClose(window);
}

void GLFWCursorPositionCallback(GLFWwindow*, double xpos, double ypos)
{
	g_input_state.prev_cursor_pos_x = g_input_state.current_cursor_pos_x;
	g_input_state.prev_cursor_pos_y = g_input_state.current_cursor_pos_y;
	g_input_state.current_cursor_pos_x = xpos;
	g_input_state.current_cursor_pos_y = ypos;
}

void GLFWKeyCallback(GLFWwindow*, int key, int, int action, int)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case(GLFW_KEY_UP):
			g_input_state.buttons_pressed_bf |= BUTTONS_PRESSED_UP_ARROW;
			break;
		case(GLFW_KEY_DOWN):
			g_input_state.buttons_pressed_bf |= BUTTONS_PRESSED_DOWN_ARROW;
			break;
		case(GLFW_KEY_RIGHT):
			g_input_state.buttons_pressed_bf |= BUTTONS_PRESSED_RIGHT_ARROW;
			break;
		case(GLFW_KEY_LEFT):
			g_input_state.buttons_pressed_bf |= BUTTONS_PRESSED_LEFT_ARROW;
			break;
		}		
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case(GLFW_KEY_UP):
			g_input_state.buttons_pressed_bf ^= BUTTONS_PRESSED_UP_ARROW;
			break;
		case(GLFW_KEY_DOWN):
			g_input_state.buttons_pressed_bf ^= BUTTONS_PRESSED_DOWN_ARROW;
			break;
		case(GLFW_KEY_RIGHT):
			g_input_state.buttons_pressed_bf ^= BUTTONS_PRESSED_RIGHT_ARROW;
			break;
		case(GLFW_KEY_LEFT):
			g_input_state.buttons_pressed_bf ^= BUTTONS_PRESSED_LEFT_ARROW;
			break;
		}
	}
}

void GLFWMouseButtonCallback(GLFWwindow*, int button, int action, int)
{
	if (action == GLFW_PRESS)
	{
		switch (button)
		{
		case(GLFW_MOUSE_BUTTON_1):
			g_input_state.buttons_pressed_bf |= BUTTONS_PRESSED_MOUSE_BUTTON_LEFT;
			break;
		case(GLFW_MOUSE_BUTTON_2):
			g_input_state.buttons_pressed_bf |= BUTTONS_PRESSED_MOUSE_BUTTON_RIGHT;
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (button)
		{
		case(GLFW_MOUSE_BUTTON_1):
			g_input_state.buttons_pressed_bf ^= BUTTONS_PRESSED_MOUSE_BUTTON_LEFT;
			break;
		case(GLFW_MOUSE_BUTTON_2):
			g_input_state.buttons_pressed_bf ^= BUTTONS_PRESSED_MOUSE_BUTTON_RIGHT;
			break;
		}
	}
}

void GLFWErrorCallback(int errorCode, const char* errorMessage)
{
	LOG("GLFW ERROR:\t");
	LOG(errorCode);
	LOG("\t");
	LOG(errorMessage);
	LOG("\n");
}

GLFW_Library::GLFW_Library()
{
	glfwSetErrorCallback(GLFWErrorCallback);

	if (!glfwInit())
	{
		LOG("GLFW_ERROR:\t\tGLFW couldn't be initialized properly\n");
	}
	else
	{
		LOG("GLFW_MSG:\t\tGLFW initialized properly\n");

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
		glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
		glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);

		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);
		glfwWindowHint(GLFW_ALPHA_BITS, 8);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_STENCIL_BITS, 8);

		glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

		glfwWindowHint(GLFW_CONTEXT_NO_ERROR, GLFW_FALSE);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rubik's Cube", NULL, NULL);

		glfwSetKeyCallback(window, GLFWKeyCallback); 
		glfwSetMouseButtonCallback(window, GLFWMouseButtonCallback);
		glfwSetCursorPosCallback(window, GLFWCursorPositionCallback);

		if (!window)
		{
			LOG("GLFW_ERROR:\t\tGLFW window couldn't be created properly\n");
			glfwTerminate();
		}
		else
		{
			LOG("GLFW_MSG:\t\tGLFW window created properly\n");

			glfwSetWindowPos(window, (1920 / 2) - (WINDOW_WIDTH / 2), 1080 - WINDOW_HEIGHT);
			glfwMakeContextCurrent(window);
			glfwSwapInterval(1);

			glfwGetFramebufferSize(window, &m_framebuffer_width, &m_framebuffer_height);
		}
	}
}

GLFW_Library::~GLFW_Library()
{
	glfwTerminate();
}
}