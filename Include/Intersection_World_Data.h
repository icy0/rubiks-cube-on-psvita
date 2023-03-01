#pragma once

#include <Mini_Cube.h>
#include <utility>

namespace Rubiks_Cube_Game
{
class Intersection_World_Data
{
private:
	Mini_Cube* m_mini_cubes[3][3][3];
	
public:
	void register_minicube(Mini_Cube& p_minicube, int p_x_index, int p_y_index, int p_z_index);
	void raycast(glm::vec3 p_ray_start, glm::vec3 p_ray_direction, Ray_Cast_Hit_Info* p_hit_info);
	// static bool is_point_on_triangle(glm::vec3 p_vertex_a, glm::vec3 p_vertex_b, glm::vec3 p_vertex_c, glm::vec3 p_point);
	static std::pair<bool, glm::vec3> is_point_on_triangle(glm::vec3 p_vertex_a, glm::vec3 p_vertex_b, glm::vec3 p_vertex_c, glm::vec3 p_ray_start, glm::vec3 p_ray_direction);
	void determine_closest_hit(Ray_Cast_Hit_Info& p_hit_info, glm::vec3 p_ray_start);
};
}
