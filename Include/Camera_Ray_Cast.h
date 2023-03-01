#pragma once

// glm headers
#include <glm.hpp>

// project internal headers
#include <Intersection_World_Data.h>

namespace Rubiks_Cube_Game
{
class Rubiks_Cube_Game_Object;

class Camera_Ray_Cast
{
private:
	Ray_Cast_Hit_Info m_ray_cast_hit_info;

	static glm::vec2 convert_to_ndc(glm::vec2 p_pixel_coords);

public:
	Camera_Ray_Cast(const Camera& p_camera, glm::vec2 p_pixel_coords, Intersection_World_Data& p_intersection_world_data);
	Camera_Ray_Cast(const Camera& p_camera, glm::vec2 p_pixel_coords, std::pair<glm::vec3, glm::vec3> p_plane);

	glm::vec3 get_ray_direction(const Camera& p_camera, glm::vec2 p_pixel_coords);
	Ray_Cast_Hit_Info& get_hit_info();
};
}