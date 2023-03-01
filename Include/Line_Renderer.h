#pragma once
// glm headers
#include <glm.hpp>

namespace Rubiks_Cube_Game
{
	class Line_Renderer
	{
	private:
		unsigned int m_vertex_buffer_id;
		unsigned int m_index_buffer_id;
		unsigned int m_shader_program_id;
		unsigned int m_uniform_mvp_id;

		glm::mat4 m_projection;
		glm::mat4 m_view;

		float m_duration;
		float m_live_for;

		float m_vertices[6];
		unsigned int m_indices[2];

	public:
		Line_Renderer(glm::vec3 p_ray_start, glm::vec3 p_ray_direction, float p_duration, float p_length, glm::mat4 p_view, glm::mat4 p_projection);
		Line_Renderer() = default;
		void update(glm::mat4 p_view);
		void render(double p_delta_time);
	};
}