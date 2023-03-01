#include <glm.hpp>
#include <Global.h>
#include <Rubiks_Cube_Game_Object.h>
#include <Camera_Ray_Cast.h>

namespace Rubiks_Cube_Game
{
/*This is a constructor, which takes the camera and uses its position and forward vec3 to cast a ray
into the game world and calculate any intersections with any game objects.*/
Camera_Ray_Cast::Camera_Ray_Cast(const Camera& p_camera, glm::vec2 p_pixel_coords, Intersection_World_Data& p_intersection_world_data)
{
	glm::vec3 ray_direction = get_ray_direction(p_camera, p_pixel_coords);
	p_intersection_world_data.raycast(p_camera.camera_pos, ray_direction, &m_ray_cast_hit_info);
}

/*This is a constructor, which takes the camera and uses its position and forward vec3 to cast a ray
into the game world and calculate any intersections with a plane.*/
Camera_Ray_Cast::Camera_Ray_Cast(const Camera& p_camera, glm::vec2 p_pixel_coords, std::pair<glm::vec3, glm::vec3> p_plane)
{
	glm::vec3 ray_direction = get_ray_direction(p_camera, p_pixel_coords);
	float distance_from_cam_to_plane = -(glm::dot(p_camera.camera_pos - p_plane.first, p_plane.second)) / glm::dot(ray_direction, p_plane.second);
	m_ray_cast_hit_info = {};
	m_ray_cast_hit_info.hit_count = 1;
	m_ray_cast_hit_info.hit_points[0] = p_camera.camera_pos + (distance_from_cam_to_plane * ray_direction);
}

/*This is a helper method which retrieves the ray direction from the camera forward vector and the left mb position.*/
glm::vec3 Camera_Ray_Cast::get_ray_direction(const Camera& p_camera, glm::vec2 p_pixel_coords)
{
	glm::vec3 ray_direction(p_camera.camera_forward);

	glm::vec2 ndc = convert_to_ndc(p_pixel_coords);

	float ray_direction_offset_angle_x = ndc.x * FOVX_DEGREES;
	float ray_direction_offset_angle_y = ndc.y * FOVY_DEGREES;

	Rubiks_Cube_Game_Object::rotate_vec3(ray_direction, p_camera.camera_up, ray_direction_offset_angle_x);
	Rubiks_Cube_Game_Object::rotate_vec3(ray_direction, p_camera.camera_right, ray_direction_offset_angle_y);

	return glm::normalize(ray_direction);
}
/*This is a helper method which converts pixel coords into normalized device coords.*/
glm::vec2 Camera_Ray_Cast::convert_to_ndc(glm::vec2 p_pixel_coords)
{
	return	glm::vec2(((p_pixel_coords.x / WINDOW_WIDTH) * 2) - 1, ((p_pixel_coords.y / WINDOW_HEIGHT) * 2) - 1);
}

/*This is a getter-method for the information of the intersection calculation.*/
Ray_Cast_Hit_Info& Camera_Ray_Cast::get_hit_info()
{
	return m_ray_cast_hit_info;
}
}