#pragma once

// glm headers
#include <glm.hpp>

// project internal headers
#include <Mini_Cube.h>
#include <Intersection_World_Data.h>
#include <Line_Renderer.h>

namespace Rubiks_Cube_Game
{
class Rubiks_Cube_Game_Object
{
private:
	glm::mat4 m_view;
	glm::mat4 m_projection;
	glm::vec3 m_cube_center;

	bool m_toggle_wf_f;
	bool m_drg_session_live;
	
	int m_drg_session_counter;
	int m_finished_drg_sessions;
	
	Dragging_Session m_current_dragging_session;
	
	Mini_Cube m_minicubes[3][3][3];

	Camera m_camera;
	Intersection_World_Data m_intersection_world_data;

	Line_Renderer m_x_axis_renderer;
	Line_Renderer m_y_axis_renderer;
	Line_Renderer m_z_axis_renderer;
	
	float m_mouse_pos_diff_x;
	float m_mouse_pos_diff_y;
	
	// OpenGL uniforms
	unsigned int m_uniform_texture_id;

public:
	Rubiks_Cube_Game_Object();
	
	void update(double p_delta_time);
	void update_camera(double p_delta_time);
	void render(double p_delta_time);
	void toggle_wireframe_fill();
	void contribute_to_drag_session();
	void start_new_drag_session(const Ray_Cast_Hit_Info& p_hit_info);
	void finish_drag_session();
	glm::vec3 filter_drag_vector(glm::vec3 p_unfiltered_drag_vector);
	void determine_affected_minicubes();

	static void rotate_vec3(glm::vec3& p_vector, glm::vec3 p_rotation_axis, float p_angle_in_deg);
	static void project(glm::vec3& p_target, glm::vec3 p_normal);
	static void orthonormalize(glm::vec3& p_target, glm::vec3 p_normal_1, glm::vec3 p_normal_2);
	static void compile_shader(const char* p_filename, unsigned int& p_shader_id, unsigned int p_shader_type);
};
}