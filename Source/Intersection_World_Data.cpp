#include <Intersection_World_Data.h>
#include <Mini_Cube.h>
#include <utility>

namespace Rubiks_Cube_Game
{
/*This is a setter-method to register all the minicubes to the intersection data.*/
void Intersection_World_Data::register_minicube(Mini_Cube& p_minicube, int p_x_index, int p_y_index, int p_z_index)
{
	m_mini_cubes[p_x_index][p_y_index][p_z_index] = &p_minicube;
}
/*This is the actual raycast-method, which performs intersection evaluation against all minicubes.*/
void Intersection_World_Data::raycast(glm::vec3 p_ray_start, glm::vec3 p_ray_direction, Ray_Cast_Hit_Info* p_hit_info)
{
	*p_hit_info = {};
	
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			for(int k = 0; k < 3; k++)
			{
				Intersection_Cube_OBB cube_OBB = m_mini_cubes[i][j][k]->get_cube_OBB();
				glm::mat4 model_matrix = m_mini_cubes[i][j][k]->get_model_matrix();

				for (int faceIndex = 0; faceIndex < 6; faceIndex++)
				{
					glm::vec4 tmp = model_matrix * glm::vec4(cube_OBB.faces[faceIndex].vertex_positions[0], 1.0f);
					glm::vec3 vertex_0 = glm::vec3(tmp.x, tmp.y, tmp.z);

					tmp = model_matrix * glm::vec4(cube_OBB.faces[faceIndex].vertex_positions[1], 1.0f);
					glm::vec3 vertex_1 = glm::vec3(tmp.x, tmp.y, tmp.z);
					
					tmp = model_matrix * glm::vec4(cube_OBB.faces[faceIndex].vertex_positions[2], 1.0f);
					glm::vec3 vertex_2 = glm::vec3(tmp.x, tmp.y, tmp.z);
					
					tmp = model_matrix * glm::vec4(cube_OBB.faces[faceIndex].vertex_positions[3], 1.0f);
					glm::vec3 vertex_3 = glm::vec3(tmp.x, tmp.y, tmp.z);

					std::pair<bool, glm::vec3> result_triangle_1 = is_point_on_triangle(vertex_0, vertex_1, vertex_2, p_ray_start, p_ray_direction);
					std::pair<bool, glm::vec3> result_triangle_2 = is_point_on_triangle(vertex_2, vertex_3, vertex_0, p_ray_start, p_ray_direction);

					if(result_triangle_1.first)
					{
						p_hit_info->hit_points[p_hit_info->hit_count] = p_ray_start + (result_triangle_1.second.z) * p_ray_direction;
						p_hit_info->hit_cubes[p_hit_info->hit_count] = glm::ivec3(i, j, k);
						p_hit_info->hit_face_index[p_hit_info->hit_count] = faceIndex;
						p_hit_info->hit_count = p_hit_info->hit_count + 1;
					}
					if (result_triangle_2.first)
					{
						p_hit_info->hit_points[p_hit_info->hit_count] = p_ray_start + (result_triangle_2.second.z) * p_ray_direction;
						p_hit_info->hit_cubes[p_hit_info->hit_count] = glm::ivec3(i, j, k);
						p_hit_info->hit_face_index[p_hit_info->hit_count] = faceIndex;
						p_hit_info->hit_count = p_hit_info->hit_count + 1;
					}
				}
			}
		}
	}
	
	determine_closest_hit(*p_hit_info, p_ray_start);
}

/*uses the Tomas Möller and Ben Trumbore algorithm*/
std::pair<bool, glm::vec3> Intersection_World_Data::is_point_on_triangle(glm::vec3 p_vertex_a, glm::vec3 p_vertex_b, glm::vec3 p_vertex_c, glm::vec3 p_ray_start, glm::vec3 p_ray_direction)	
{
	glm::vec3 e1 = p_vertex_b - p_vertex_a;
	glm::vec3 e2 = p_vertex_c - p_vertex_a;
	glm::vec3 q = glm::cross(p_ray_direction, e2);
	float a = glm::dot(e1, q);
	
	if(a > -FLT_EPSILON && a < FLT_EPSILON)
		return std::pair<bool, glm::vec3>(false, glm::vec3(0));

	float f = 1 / a;
	glm::vec3 s = p_ray_start - p_vertex_a;
	float u = f * (glm::dot(s, q));

	if (u < 0.0f)
		return std::pair<bool, glm::vec3>(false, glm::vec3(0));

	glm::vec3 r = glm::cross(s, e1);
	float v = f * (glm::dot(p_ray_direction , r));

	if ((v < 0.0f) || ((u + v) > 1.0f))
		return std::pair<bool, glm::vec3>(false, glm::vec3(0));

	float t = f * (glm::dot(e2, r));
	return { true, glm::vec3(u, v, t) };
}

/*determines, which of the intersection points is the one closest to the ray start.*/
void Intersection_World_Data::determine_closest_hit(Ray_Cast_Hit_Info& p_hit_info, glm::vec3 p_ray_start)
{
	glm::vec3 closest_hit_point = glm::vec3(1000.0f, 1000.0f, 1000.0f);
	// PORT set initial closest_hit_cube to -1-1-1
	glm::ivec3 closest_hit_cube = glm::ivec3(-1, -1, -1);

	int closest_hit_index = 0;
	for (int hit_cube_index = 0; hit_cube_index < p_hit_info.hit_count; hit_cube_index++)
	{
		if (glm::length(closest_hit_point - p_ray_start) > glm::length(p_hit_info.hit_points[hit_cube_index] - p_ray_start))
		{
			closest_hit_point = p_hit_info.hit_points[hit_cube_index];
			closest_hit_cube = p_hit_info.hit_cubes[hit_cube_index];
			closest_hit_index = hit_cube_index;
		}
	}

	p_hit_info.closest_hit_hitindex = closest_hit_index;
	p_hit_info.closest_hit_indices = closest_hit_cube;
	p_hit_info.closest_hit_point = closest_hit_point;
}
}