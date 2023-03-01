// glfw headers
#include <GLFW/glfw3.h>

// imgui headers
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// project internal headers
#include <GLEW_Library.h>
#include <GLFW_Library.h>
#include <Rubiks_Cube_Game_Object.h>
#include <Global.h>
#include <Logging.h>

using namespace Rubiks_Cube_Game;
Input_State g_input_state = {};

int main()
{
	// initialize RAII wrappers for the libraries.
	GLFW_Library glfw;
	GLEW_Library glew;
	
	Rubiks_Cube_Game_Object rc_game_object = Rubiks_Cube_Game_Object();
	double delta_frame_time = 0;
	double fps = 0;

	// initialize debug outputs
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(&(glfw.get_window()), true);
	ImGui_ImplOpenGL3_Init("#version 330");
	
	while (!glfw.window_should_close())
	{
		const double frame_start_time = glfwGetTime();

		// poll inputs
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		rc_game_object.update(delta_frame_time);
		rc_game_object.render(delta_frame_time);

		// debug outputs
		ImGui::Begin("FPS");

		char buffer[64];
		ImGui::Text("FPS: ");
		ImGui::SameLine();
		snprintf(buffer, sizeof(buffer), "%f", fps);
		ImGui::Text(buffer);

		ImGui::End();

		// set bitfield of input data
		if (g_input_state.buttons_pressed_bf & BUTTONS_PRESSED_MOUSE_BUTTON_LEFT) // if left mb is down (every time)
		{
			g_input_state.buttons_pressed_bf |= MOUSE_BUTTON_LEFT_WAS_DOWN;
		}
		else if(g_input_state.buttons_pressed_bf & MOUSE_BUTTON_LEFT_WAS_DOWN) // if left mb is let go (only once)
		{
			g_input_state.buttons_pressed_bf ^= MOUSE_BUTTON_LEFT_WAS_DOWN;
			g_input_state.touch_id++;
;		}

		// debug outputs
		ImGui::Begin("Input");

		ImGui::Text("MB_LEFT: ");
		ImGui::SameLine();
		snprintf(buffer, sizeof(buffer), "%i", (g_input_state.buttons_pressed_bf & BUTTONS_PRESSED_MOUSE_BUTTON_LEFT));
		ImGui::Text(buffer);
		
		ImGui::Text("MB_LEFT_WAS_DOWN: ");
		ImGui::SameLine();
		snprintf(buffer, sizeof(buffer), "%i", (g_input_state.buttons_pressed_bf & MOUSE_BUTTON_LEFT_WAS_DOWN));
		ImGui::Text(buffer);
		
		ImGui::End();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		// cycle display buffers
		glfwSwapBuffers(&(glfw.get_window()));

		delta_frame_time = glfwGetTime() - frame_start_time;
		fps = (1.0 / delta_frame_time);
	}

	// clean-up
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}