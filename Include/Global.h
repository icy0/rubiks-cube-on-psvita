#pragma once

#include <glm.hpp>
#include <utility>

#define M_PI 3.14159f

#define WINDOW_WIDTH 960.0f
#define WINDOW_HEIGHT 544.0f
#define CAMERA_ZNEAR 0.1f
#define CAMERA_ZFAR 100.0f
#define CAMERA_DISTANCE_TO_CUBE_CENTER 15.0f
#define FOVY glm::radians(45.0f)
#define FOVX (((FOVY) / (WINDOW_HEIGHT)) * (WINDOW_WIDTH))

#define FOVY_DEGREES 45.0f
#define FOVX_DEGREES (((FOVY_DEGREES) / (WINDOW_HEIGHT)) * (WINDOW_WIDTH))

namespace Rubiks_Cube_Game
{
enum BUTTONS_PRESSED_BITFIELD
{
	BUTTONS_PRESSED_UP_ARROW = 1,
	BUTTONS_PRESSED_DOWN_ARROW = 2,
	BUTTONS_PRESSED_RIGHT_ARROW = 4,
	BUTTONS_PRESSED_LEFT_ARROW = 8,
	BUTTONS_PRESSED_MOUSE_BUTTON_LEFT = 16,
	BUTTONS_PRESSED_MOUSE_BUTTON_RIGHT = 32,
	MOUSE_BUTTON_LEFT_WAS_DOWN = 64,
};

struct Dragging_Session
{
	glm::vec3 starting_point;
	glm::vec3 current_point;
	glm::vec3 unfiltered_drag_vector;
	glm::vec3 filtered_drag_vector;
	std::pair<glm::vec3, glm::vec3> plane;
	glm::ivec3 target_cube_indices;
	glm::vec3 drag_axis;
	glm::ivec3 affected_minicubes[9];
};

struct Input_State
{
	unsigned char buttons_pressed_bf;
	int current_cursor_pos_x;
	int current_cursor_pos_y;
	int prev_cursor_pos_x;
	int prev_cursor_pos_y;
	unsigned int touch_id;
};

struct Ray_Cast_Hit_Info
{
	glm::vec3 hit_points[100];			// All the 3D points where the ray intersected any plane.
	glm::ivec3 hit_cubes[100];			// All the cube indices that match the rubiks cube gameobject datastructure.
	int hit_face_index[100];			// All the indices of the faces intersected by the ray.
	int hit_count;						// Amount of intersections.
	int closest_hit_hitindex;			// Index of hit_points of closest hit_point.
	glm::ivec3 closest_hit_indices;		// The cube indices of the closest hit cube that match the rubiks cube gameobject datastructure.
	glm::vec3 closest_hit_point;		// The closest 3D hit point.
};

struct Camera
{
	glm::vec3 camera_pos;
	glm::vec3 camera_up;
	glm::vec3 camera_right;
	glm::vec3 camera_forward;
};

struct Intersection_Cube_OBB
{
struct Cube_Face
{
	glm::vec3 vertex_positions[4];
	glm::vec3 normal;
};

Cube_Face faces[6] =
{
	// Cube_Face 1
	{
		{
			{-1.0f, -1.0f, 1.0f},
			{-1.0f, 1.0f, 1.0f},
			{-1.0f, 1.0f, -1.0f},
			{-1.0f, -1.0f, -1.0f}
		},
	{-1.0f, 0.0f, 0.0f}
	},

	// Cube_Face 2
	{
		{
			{-1.0f, -1.0f, -1.0f},
			{-1.0f, 1.0f, -1.0f},
			{1.0f, 1.0f, -1.0f},
			{1.0f, -1.0f, -1.0f}
		},
		{
			0.0f, 0.0f, -1.0f
		}
	},

	// Cube_Face 3
	{
		{
			{1.0f, -1.0f, -1.0f},
			{1.0f, 1.0f, -1.0f},
			{1.0f, 1.0f, 1.0f},
			{1.0f, -1.0f, 1.0f}
		},
		{
			1.0f, 0.0f, 0.0f
		}
	},

	// Cube_Face 4
	{
		{
			{1.0f, -1.0f, 1.0f},
			{1.0f, 1.0f, 1.0f},
			{-1.0f, 1.0f, 1.0f},
			{-1.0f, -1.0f, 1.0f}
		},
		{
			0.0f, 0.0f, 1.0f
		}
	},

	// Cube_Face 5
	{
		{
			{-1.0f, -1.0f, -1.0f},
			{1.0f, -1.0f, -1.0f},
			{1.0f, -1.0f, 1.0f},
			{-1.0f, -1.0f, 1.0f}
		},
		{
			0.0f, -1.0f, 0.0f
		}
	},

	// Cube_Face 6
	{
		{
			{1.0f, 1.0f, -1.0f},
			{-1.0f, 1.0f, -1.0f},
			{-1.0f, 1.0f, 1.0f},
			{1.0f, 1.0f, 1.0f}
		},
		{
			0.0f, 1.0f, 0.0f
		}
	}
};
};
}

extern Rubiks_Cube_Game::Input_State g_input_state;
