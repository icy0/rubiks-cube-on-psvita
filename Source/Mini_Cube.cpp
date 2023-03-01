// glew headers
#include <GL/glew.h>

// project internal headers
#include <Mini_Cube.h>
#include <Rubiks_Cube_Game_Object.h>

namespace Rubiks_Cube_Game
{
/*This is the constructor of the minicube.*/
Mini_Cube::Mini_Cube(glm::mat4 p_model_matrix)
{
	// initialize members
	this->m_model = p_model_matrix;
	this->m_hitflag = 0;

	this->m_drag_vector_length = 0;
	this->m_drag_axis = glm::vec3(0);

	// generate vertex and index buffers
	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// build shaders
	unsigned int vertex_shader;
	Rubiks_Cube_Game_Object::compile_shader("Resources/shader.vert", vertex_shader, GL_VERTEX_SHADER);

	unsigned int fragment_shader;
	Rubiks_Cube_Game_Object::compile_shader("Resources/shader.frag", fragment_shader, GL_FRAGMENT_SHADER);

	m_shader_program = glCreateProgram();

	glAttachShader(m_shader_program, vertex_shader);
	glAttachShader(m_shader_program, fragment_shader);

	glLinkProgram(m_shader_program);
	glValidateProgram(m_shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	
	// retrieve uniform location from the shaders
	m_uniform_model_id = glGetUniformLocation(m_shader_program, "u_model");
	m_uniform_view_id = glGetUniformLocation(m_shader_program, "u_view");
	m_uniform_projection_id = glGetUniformLocation(m_shader_program, "u_projection");
	m_uniform_hitflag_id = glGetUniformLocation(m_shader_program, "u_hitflag");
	m_uniform_drag_vector_length_id = glGetUniformLocation(m_shader_program, "u_drag_vector_length");
	m_uniform_drag_axis_id = glGetUniformLocation(m_shader_program, "u_drag_axis");
}

/*This is the method, which performs the actual drawcalls to render the minicubes.*/
void Mini_Cube::render(glm::mat4 p_projection, glm::mat4 p_view)
{
	m_view = p_view;
	m_projection = p_projection;
	const int stride = 8 * sizeof(float);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

	glUseProgram(m_shader_program);

	glUniformMatrix4fv(m_uniform_model_id, 1, GL_FALSE, glm::value_ptr(m_model));
	glUniformMatrix4fv(m_uniform_view_id, 1, GL_FALSE, glm::value_ptr(m_view));
	glUniformMatrix4fv(m_uniform_projection_id, 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniform1i(m_uniform_hitflag_id, m_hitflag);
	glUniform1f(m_uniform_drag_vector_length_id, m_drag_vector_length);
	glUniform3f(m_uniform_drag_axis_id, m_drag_axis.x, m_drag_axis.y, m_drag_axis.z);

	glDrawElements(GL_TRIANGLES, (sizeof(indices) / sizeof(unsigned int)), GL_UNSIGNED_INT, nullptr);

	m_hitflag = 0;
}

void Mini_Cube::set_hit_flag()
{
	m_hitflag = 1;
}
void Mini_Cube::set_drag_vector_length(float p_drag_vector_length)
{
	m_drag_vector_length = p_drag_vector_length;
}
	
void Mini_Cube::set_drag_axis(glm::vec3 p_drag_axis)
{
	m_drag_axis = p_drag_axis;
}
}