#pragma once

#include <ext.hpp>
#include <glm.hpp>

#include <Global.h>

namespace Rubiks_Cube_Game
{
class Mini_Cube
{
private:
	Intersection_Cube_OBB m_cube_OBB;

	int m_hitflag;
	float m_drag_vector_length;
	glm::vec3 m_drag_axis;

	glm::mat4 m_model;
	glm::mat4 m_view;
	glm::mat4 m_projection;
	
	unsigned int m_vertex_buffer;
	unsigned int m_index_buffer;
	unsigned int m_shader_program;
	
	unsigned int m_uniform_model_id;
	unsigned int m_uniform_view_id;
	unsigned int m_uniform_projection_id;
	unsigned int m_uniform_hitflag_id;
	unsigned int m_uniform_drag_vector_length_id;
	unsigned int m_uniform_drag_axis_id;
	

	float vertices[192] =
	{
		//		position:				texcoords:			normals:
				-1.0f, -1.0f,  1.0f,	0.375f, 0.0f,		-1.0f, 0.0f, 0.0f,	//
				-1.0f,	1.0f,  1.0f,	0.625f, 0.0f,		-1.0f, 0.0f, 0.0f,	//	face 1
				-1.0f,	1.0f, -1.0f,	0.625f, 0.25f,		-1.0f, 0.0f, 0.0f,	//
				-1.0f, -1.0f, -1.0f,	0.375f, 0.25f,		-1.0f, 0.0f, 0.0f,	//


				-1.0f, -1.0f, -1.0f,	0.375f, 0.25f,		0.0f, 0.0f, -1.0f,	//
				-1.0f,  1.0f, -1.0f,	0.625f, 0.25f,		0.0f, 0.0f, -1.0f,	//	face 2
				 1.0f,  1.0f, -1.0f,	0.625f, 0.5f,		0.0f, 0.0f, -1.0f,	//
				 1.0f, -1.0f, -1.0f,	0.375f, 0.5f,		0.0f, 0.0f, -1.0f,	//


				 1.0f, -1.0f, -1.0f,	0.375f, 0.5f,		1.0f, 0.0f, 0.0f,	//
				 1.0f,  1.0f, -1.0f,	0.625f, 0.5f,		1.0f, 0.0f, 0.0f,	//	face 3
				 1.0f,  1.0f,  1.0f,	0.625f, 0.75f,		1.0f, 0.0f, 0.0f,	//
				 1.0f, -1.0f,  1.0f,	0.375f, 0.75f,		1.0f, 0.0f, 0.0f,	//


				 1.0f, -1.0f,  1.0f,	0.375f, 0.75f,		0.0f, 0.0f, 1.0f,	//
				 1.0f,  1.0f,  1.0f,	0.625f, 0.75f,		0.0f, 0.0f, 1.0f,	//	face 4
				-1.0f,  1.0f,  1.0f,	0.625f, 1.0f,		0.0f, 0.0f, 1.0f,	//
				-1.0f, -1.0f,  1.0f,	0.375f, 1.0f,		0.0f, 0.0f, 1.0f,	//


				-1.0f, -1.0f, -1.0f,	0.125f, 0.5f,		0.0f, -1.0f, 0.0f,	//
				 1.0f, -1.0f, -1.0f,	0.375f, 0.5f,		0.0f, -1.0f, 0.0f,	//	face 5
				 1.0f, -1.0f,  1.0f,	0.375f, 0.75f,		0.0f, -1.0f, 0.0f,	//
				-1.0f, -1.0f,  1.0f,	0.125f, 0.75f,		0.0f, -1.0f, 0.0f,	//


				 1.0f,  1.0f, -1.0f,	0.625f, 0.5f,		0.0f, 1.0f, 0.0f,	//
				-1.0f,  1.0f, -1.0f,	0.875f, 0.5f,		0.0f, 1.0f, 0.0f,	//	face 6
				-1.0f,  1.0f,  1.0f,	0.875f, 0.75f,		0.0f, 1.0f, 0.0f,	//
				 1.0f,  1.0f,  1.0f,	0.625f, 0.75f,		0.0f, 1.0f, 0.0f,	//
	};

	unsigned int indices[36] =
	{
		// face 1
		0, 1, 2,
		2, 3, 0,

		// face 2
		4, 5, 6,
		6, 7, 4,

		// face 3
		8, 9, 10,
		10, 11, 8,

		// face 4
		12, 13, 14,
		14, 15, 12,

		// face 5
		16, 17, 18,
		18, 19, 16,

		// face 6
		20, 21, 22,
		22, 23, 20,
	};

public:
	Mini_Cube() {}
	Mini_Cube(glm::mat4 p_model_matrix);

	glm::mat4& get_model_matrix() { return this->m_model; }
	Intersection_Cube_OBB get_cube_OBB() { return this->m_cube_OBB; }

	void render(glm::mat4 p_projection, glm::mat4 p_view);
	void set_hit_flag();

	void set_drag_vector_length(float p_drag_vector_length);
	void set_drag_axis(glm::vec3 p_drag_axis);
};
}