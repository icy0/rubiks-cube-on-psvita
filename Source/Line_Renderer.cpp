// glew headers
#include <GL/glew.h>

// glm headers
#include <glm.hpp>

// project internal headers
#include <Line_Renderer.h>
#include <Rubiks_Cube_Game_Object.h>

namespace Rubiks_Cube_Game
{
Line_Renderer::Line_Renderer(glm::vec3 p_ray_start, glm::vec3 p_ray_direction, float p_duration, float p_length, glm::mat4 p_view, glm::mat4 p_projection)
{
	m_view = p_view;
	m_projection = p_projection;

	p_ray_direction = glm::normalize(p_ray_direction);
	glm::vec3 ray_end = p_ray_start + (p_ray_direction * p_length);
	
	m_vertices[0] = p_ray_start.x;
	m_vertices[1] = p_ray_start.y;
	m_vertices[2] = p_ray_start.z;
	m_vertices[3] = ray_end.x;
	m_vertices[4] = ray_end.y;
	m_vertices[5] = ray_end.z;

	m_indices[0] = 0;
	m_indices[1] = 1;

	m_duration = p_duration;
	m_live_for = 0.0f;

	glGenBuffers(1, &m_vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_index_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

	unsigned int vertex_shader;
	Rubiks_Cube_Game_Object::compile_shader("Resources/ray.vert", vertex_shader, GL_VERTEX_SHADER);

	unsigned int fragment_shader;
	Rubiks_Cube_Game_Object::compile_shader("Resources/ray.frag", fragment_shader, GL_FRAGMENT_SHADER);

	m_shader_program_id = glCreateProgram();

	glAttachShader(m_shader_program_id, vertex_shader);
	glAttachShader(m_shader_program_id, fragment_shader);

	glLinkProgram(m_shader_program_id);
	glValidateProgram(m_shader_program_id);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	
	m_uniform_mvp_id = glGetUniformLocation(m_shader_program_id, "u_mvp");
}

void Line_Renderer::update(glm::mat4 p_view)
{
	m_view = p_view;
}

void Line_Renderer::render(double p_delta_time)
{
	m_live_for += p_delta_time;

	if((m_duration == 0.0) || (m_live_for <= m_duration))
	{
		glm::mat4 mvp = m_projection * m_view;
	
		const int stride = 3 * sizeof(float);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_id);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

		glUseProgram(m_shader_program_id);
		glUniformMatrix4fv(m_uniform_mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));

		glDrawElements(GL_LINES, (sizeof(m_indices) / sizeof(unsigned int)), GL_UNSIGNED_INT, nullptr);
	}
}
}