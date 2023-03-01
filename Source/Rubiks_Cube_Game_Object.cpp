// glew headers
#include <GL/glew.h>

// glm headers
#include <glm.hpp>
#include <ext.hpp>

// imgui headers
#include <imgui.h>

// stb headers
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// cstdlib headers
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// project internal headers
#include <Rubiks_Cube_Game_Object.h>
#include <Camera_Ray_Cast.h>
#include <Mini_Cube.h>
#include <Global.h>
#include <Logging.h>


namespace Rubiks_Cube_Game
{
/*OpenGL error callback. Logs the error to the project internal LOG-macro.*/
void __stdcall GLErrorCallback(GLenum, GLenum, GLuint, GLenum, GLsizei, 
const GLchar* message, const void*)
{
	LOG("OPENGL:\t\t");
	LOG(message);
	LOG("\n");
}

/*The main constructor, initializing the members and initializing / setting up OpenGL.
It doesn't use an initializer list, because it is supposed to compile with c++03.*/
Rubiks_Cube_Game_Object::Rubiks_Cube_Game_Object()
{
	// initialize members
	m_intersection_world_data = Intersection_World_Data();
	
	m_mouse_pos_diff_x = 0.0f;
	m_mouse_pos_diff_y = 0.0f;

	m_toggle_wf_f = true;
	m_drg_session_live = false;
	m_current_dragging_session = {};
	m_drg_session_counter = 0;
	m_finished_drg_sessions = 0;

	
	// initialize mini-cubes
	const float translation = 2.1f;
	for (int x_index = 0; x_index < 3; x_index++)
	{
		for (int y_index = 0; y_index < 3; y_index++)
		{
			for (int z_index = 0; z_index < 3; z_index++)
			{
				glm::vec3 translation_vector =
					glm::vec3((x_index - 1) * translation,
					(y_index - 1) * translation,
					(z_index - 1) * translation);
				
				glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), translation_vector);
				
				m_minicubes[x_index][y_index][z_index] = Mini_Cube(translation_matrix);

				m_intersection_world_data.register_minicube(m_minicubes[x_index][y_index][z_index], x_index, y_index, z_index);
			}
		}
	}

	
	// initialize camera
	m_cube_center = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 tmp_camera_forward = m_cube_center - glm::vec3(0.0f, 0.0f, CAMERA_DISTANCE_TO_CUBE_CENTER);
	tmp_camera_forward = glm::normalize(tmp_camera_forward);
	
	m_camera = {};
	m_camera.camera_pos = glm::vec3(0.0f, 0.0f, CAMERA_DISTANCE_TO_CUBE_CENTER);
	m_camera.camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_camera.camera_right = glm::vec3(1.0f, 0.0f, 0.0f);
	m_camera.camera_forward = tmp_camera_forward;
	
	m_view = glm::lookAt(m_camera.camera_pos, m_cube_center, m_camera.camera_up);
	m_projection = glm::perspective(FOVY, (float)WINDOW_WIDTH / WINDOW_HEIGHT, CAMERA_ZNEAR, CAMERA_ZFAR);

	
	// initialize coordinate-axis rays
	m_x_axis_renderer = Line_Renderer(glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), 0.0, 200.0f, m_view, m_projection);
	m_y_axis_renderer = Line_Renderer(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 0.0, 200.0f, m_view, m_projection);
	m_z_axis_renderer = Line_Renderer(glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(0.0f, 0.0f, -1.0f), 0.0, 200.0f, m_view, m_projection);

	
	// initialize OpenGL textures
	int width, height, nrChannels;
	unsigned char* data = stbi_load("Resources/Material.001 Base Color2.png", &width, &height, &nrChannels, 0);

	glGenTextures(1, &m_uniform_texture_id);
	glBindTexture(GL_TEXTURE_2D, m_uniform_texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);
	glDebugMessageCallback(GLErrorCallback, nullptr);

	unsigned int dummyVAO;
	glGenVertexArrays(1, &dummyVAO);
	glBindVertexArray(dummyVAO);

	stbi_image_free(data);
}

/*This method uses the collected backtouch and joystick input data to manipulate the camera.*/
void Rubiks_Cube_Game_Object::update_camera(double p_delta_time)
{
	const float camera_speed = 100.0f * (float)p_delta_time;

	// evaluate input
	if (g_input_state.buttons_pressed_bf & BUTTONS_PRESSED_UP_ARROW)
	{
		rotate_vec3(m_camera.camera_up, m_camera.camera_right, -camera_speed);
		project(m_camera.camera_up, m_camera.camera_right);
		rotate_vec3(m_camera.camera_forward, m_camera.camera_right, -camera_speed);
		project(m_camera.camera_forward, m_camera.camera_right);
		rotate_vec3(m_camera.camera_pos, m_camera.camera_right, -camera_speed);
	}
	if (g_input_state.buttons_pressed_bf & BUTTONS_PRESSED_DOWN_ARROW)
	{
		rotate_vec3(m_camera.camera_up, m_camera.camera_right, camera_speed);
		project(m_camera.camera_up, m_camera.camera_right);
		rotate_vec3(m_camera.camera_forward, m_camera.camera_right, camera_speed);
		project(m_camera.camera_forward, m_camera.camera_right);
		rotate_vec3(m_camera.camera_pos, m_camera.camera_right, camera_speed);
	}
	if (g_input_state.buttons_pressed_bf & BUTTONS_PRESSED_RIGHT_ARROW)
	{
		rotate_vec3(m_camera.camera_right, m_camera.camera_up, camera_speed);
		project(m_camera.camera_right, m_camera.camera_up);
		rotate_vec3(m_camera.camera_forward, m_camera.camera_up, camera_speed);
		project(m_camera.camera_forward, m_camera.camera_up);
		rotate_vec3(m_camera.camera_pos, m_camera.camera_up, camera_speed);
	}
	if (g_input_state.buttons_pressed_bf & BUTTONS_PRESSED_LEFT_ARROW)
	{
		rotate_vec3(m_camera.camera_right, m_camera.camera_up, -camera_speed);
		project(m_camera.camera_right, m_camera.camera_up);
		rotate_vec3(m_camera.camera_forward, m_camera.camera_up, -camera_speed);
		project(m_camera.camera_forward, m_camera.camera_up);
		rotate_vec3(m_camera.camera_pos, m_camera.camera_up, -camera_speed);
	}

	if (g_input_state.buttons_pressed_bf & BUTTONS_PRESSED_MOUSE_BUTTON_RIGHT)
	{
		rotate_vec3(m_camera.camera_pos, m_camera.camera_up, m_mouse_pos_diff_x);
		rotate_vec3(m_camera.camera_pos, m_camera.camera_right, m_mouse_pos_diff_y);

		rotate_vec3(m_camera.camera_up, m_camera.camera_right, m_mouse_pos_diff_y);
		rotate_vec3(m_camera.camera_right, m_camera.camera_up, m_mouse_pos_diff_x);
		orthonormalize(m_camera.camera_forward, m_camera.camera_up, m_camera.camera_right);
	}

	// normalize to eliminate accumulating error
	m_camera.camera_pos = glm::normalize(m_camera.camera_pos);
	m_camera.camera_pos *= CAMERA_DISTANCE_TO_CUBE_CENTER;

	// update the view according to the new camera position and new camera_up-vector.
	m_view = glm::lookAt(m_camera.camera_pos, m_cube_center, m_camera.camera_up);
}
	
/*The update-method evaluates the input data and updates all members.*/
void Rubiks_Cube_Game_Object::update(double p_delta_time)
{
	// update the difference vector of the mouse movement
	m_mouse_pos_diff_x = (float)g_input_state.current_cursor_pos_x - (float)g_input_state.prev_cursor_pos_x;
	m_mouse_pos_diff_y = (float)g_input_state.current_cursor_pos_y - (float)g_input_state.prev_cursor_pos_y;
	
	Ray_Cast_Hit_Info hit_info = {};
	glm::vec2 mouse_position = glm::vec2(g_input_state.current_cursor_pos_x, g_input_state.current_cursor_pos_y);

	if (g_input_state.buttons_pressed_bf & BUTTONS_PRESSED_MOUSE_BUTTON_LEFT) // if left mb down
	{
		if (!m_drg_session_live) // if this is the first frame of the left mb being down
		{
			// shoot a ray
			Camera_Ray_Cast raycast(m_camera, mouse_position, m_intersection_world_data);
			hit_info = raycast.get_hit_info();

			if ((hit_info.hit_count > 0) && !(g_input_state.buttons_pressed_bf & MOUSE_BUTTON_LEFT_WAS_DOWN)) // if we hit something on the first frame of holding down left mb
				start_new_drag_session(hit_info);
		}
		
		if (m_drg_session_live) // if this is not the first frame of the left mb being down
			contribute_to_drag_session();

		// debug visuals
		if(hit_info.hit_count > 0)
			m_minicubes[hit_info.closest_hit_indices.x][hit_info.closest_hit_indices.y][hit_info.closest_hit_indices.z].set_hit_flag();
		
	}
	else // if left mb is not down
	{
		if(g_input_state.buttons_pressed_bf & MOUSE_BUTTON_LEFT_WAS_DOWN) // if left mb was just let go
		{
			finish_drag_session();
		}
		
		update_camera(p_delta_time); // can only be moved if there is no fronttouch input
	}

	// this updates the lines which represent the axis of our coordinate system
	m_x_axis_renderer.update(m_view);
	m_y_axis_renderer.update(m_view);
	m_z_axis_renderer.update(m_view);

	// debug outputs
	ImGui::Begin("Hit-Info");

	char buffer[64];
	ImGui::Text("Hit-Count: ");
	ImGui::SameLine();
	snprintf(buffer, sizeof buffer, "%i", hit_info.hit_count);
	ImGui::Text(buffer);

	ImGui::End();
	
	ImGui::Begin("Camera");	

	ImGui::Text("Camera-Pos: ");

	ImGui::SameLine();
	snprintf(buffer, sizeof buffer, "%f, %f, %f", m_camera.camera_pos.x, m_camera.camera_pos.y, m_camera.camera_pos.z);
	ImGui::Text(buffer);

	ImGui::Text("Camera-Up: ");

	ImGui::SameLine();
	snprintf(buffer, sizeof buffer, "%f, %f, %f", m_camera.camera_up.x, m_camera.camera_up.y, m_camera.camera_up.z);
	ImGui::Text(buffer);

	ImGui::Text("Camera-Forward: ");

	ImGui::SameLine();
	snprintf(buffer, sizeof buffer, "%f, %f, %f", m_camera.camera_forward.x, m_camera.camera_forward.y, m_camera.camera_forward.z);
	ImGui::Text(buffer);

	ImGui::Text("Camera-Right: ");

	ImGui::SameLine();
	snprintf(buffer, sizeof buffer, "%f, %f, %f", m_camera.camera_right.x, m_camera.camera_right.y, m_camera.camera_right.z);
	ImGui::Text(buffer);

	ImGui::End();

	ImGui::Begin("Wireframe / Fill");
	if (ImGui::Button("Toggle", ImVec2(100.0f, 20.0f)))
	{
		toggle_wireframe_fill();
	}
	ImGui::End();

	ImGui::Begin("Drag-Sessions");

	ImGui::Text("Active: ");

	ImGui::SameLine();
	snprintf(buffer, sizeof buffer, "%s", m_drg_session_live? "true" : "false");
	ImGui::Text(buffer);

	ImGui::Text("Sessions started: ");

	ImGui::SameLine();
	snprintf(buffer, sizeof buffer, "%d", m_drg_session_counter);
	ImGui::Text(buffer);

	ImGui::Text("Sessions finished: ");

	ImGui::SameLine();
	snprintf(buffer, sizeof buffer, "%d", m_finished_drg_sessions);
	ImGui::Text(buffer);
	
	ImGui::End();
}

/*This is a helper-method for the update-method. 
 It takes the hit info from the raycast and starts a new dragging session.*/
void Rubiks_Cube_Game_Object::start_new_drag_session(const Ray_Cast_Hit_Info& p_hit_info)
{
	m_drg_session_live = true;
	m_current_dragging_session = {};
	m_current_dragging_session.starting_point = p_hit_info.closest_hit_point;

	Mini_Cube* mini_cube = &m_minicubes[p_hit_info.closest_hit_indices.x][p_hit_info.closest_hit_indices.y][p_hit_info.closest_hit_indices.z];
	glm::vec3 plane_normal = mini_cube->get_cube_OBB().faces[p_hit_info.hit_face_index[p_hit_info.closest_hit_hitindex]].normal;
	plane_normal = mini_cube->get_model_matrix() * glm::vec4(plane_normal, 0.0f);
	glm::vec3 point_on_plane = p_hit_info.closest_hit_point;

	m_current_dragging_session.plane = std::pair<glm::vec3, glm::vec3>(point_on_plane, plane_normal);
	m_current_dragging_session.target_cube_indices = p_hit_info.closest_hit_indices;

	m_drg_session_counter++;
}

/*This is a helper-method for the contribute_to_drag_session-method. 
It determines all the minicubes which are affected by a drag.*/
void Rubiks_Cube_Game_Object::determine_affected_minicubes()
{
	int list_size = 0;
	glm::vec3 tmp_drag_vector = glm::normalize(m_current_dragging_session.filtered_drag_vector);
	glm::ivec3 indices = m_current_dragging_session.target_cube_indices;
	glm::vec3 position_hit_cube = m_minicubes[indices.x][indices.y][indices.z].get_model_matrix()[3];
	glm::vec3 norm_pos_hit_cube;

	if (position_hit_cube.x > 0.1f)
		norm_pos_hit_cube.x = 1.0f;
	if (position_hit_cube.x <= 0.1f && position_hit_cube.x >= -0.1f)
		norm_pos_hit_cube.x = 0.0f;
	if (position_hit_cube.x < -0.1f)
		norm_pos_hit_cube.x = -1.0f;

	if (position_hit_cube.y > 0.1f)
		norm_pos_hit_cube.y = 1.0f;
	if (position_hit_cube.y <= 0.1f && position_hit_cube.y >= -0.1f)
		norm_pos_hit_cube.y = 0.0f;
	if (position_hit_cube.y < -0.1f)
		norm_pos_hit_cube.y = -1.0f;

	if (position_hit_cube.z > 0.1f)
		norm_pos_hit_cube.z = 1.0f;
	if (position_hit_cube.z <= 0.1f && position_hit_cube.z >= -0.1f)
		norm_pos_hit_cube.z = 0.0f;
	if (position_hit_cube.z < -0.1f)
		norm_pos_hit_cube.z = -1.0f;


	glm::vec3 dragging_plane_normal = m_current_dragging_session.plane.second;
	glm::vec3 norm_plane_normal;

	if (dragging_plane_normal.x > 0.1f)
		norm_plane_normal.x = 1.0f;
	if (dragging_plane_normal.x <= 0.1f && dragging_plane_normal.x >= -0.1f)
		norm_plane_normal.x = 0.0f;
	if (dragging_plane_normal.x < -0.1f)
		norm_plane_normal.x = -1.0f;

	if (dragging_plane_normal.y > 0.1f)
		norm_plane_normal.y = 1.0f;
	if (dragging_plane_normal.y <= 0.1f && dragging_plane_normal.y >= -0.1f)
		norm_plane_normal.y = 0.0f;
	if (dragging_plane_normal.y < -0.1f)
		norm_plane_normal.y = -1.0f;

	if (dragging_plane_normal.z > 0.1f)
		norm_plane_normal.z = 1.0f;
	if (dragging_plane_normal.z <= 0.1f && dragging_plane_normal.z >= -0.1f)
		norm_plane_normal.z = 0.0f;
	if (dragging_plane_normal.z < -0.1f)
		norm_plane_normal.z = -1.0f;

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int z = 0; z < 3; z++)
			{
				glm::vec3 position_current_cube = m_minicubes[x][y][z].get_model_matrix()[3];
				glm::vec3 norm_pos_current_cube;

				if (position_current_cube.x > 0.1f)
					norm_pos_current_cube.x = 1.0f;
				if (position_current_cube.x <= 0.1f && position_current_cube.x >= -0.1f)
					norm_pos_current_cube.x = 0.0f;
				if (position_current_cube.x < -0.1f)
					norm_pos_current_cube.x = -1.0f;

				if (position_current_cube.y > 0.1f)
					norm_pos_current_cube.y = 1.0f;
				if (position_current_cube.y <= 0.1f && position_current_cube.y >= -0.1f)
					norm_pos_current_cube.y = 0.0f;
				if (position_current_cube.y < -0.1f)
					norm_pos_current_cube.y = -1.0f;

				if (position_current_cube.z > 0.1f)
					norm_pos_current_cube.z = 1.0f;
				if (position_current_cube.z <= 0.1f && position_current_cube.z >= -0.1f)
					norm_pos_current_cube.z = 0.0f;
				if (position_current_cube.z < -0.1f)
					norm_pos_current_cube.z = -1.0f;
				
				
				if ((tmp_drag_vector.x != 0 && norm_plane_normal.y != 0)
					&& (norm_pos_current_cube.z == norm_pos_hit_cube.z))
				{
					m_current_dragging_session.affected_minicubes[list_size] = glm::ivec3(x, y, z);
					list_size++;
				}
				else if ((tmp_drag_vector.x != 0 && norm_plane_normal.z != 0)
					&& (norm_pos_current_cube.y == norm_pos_hit_cube.y))
				{
					m_current_dragging_session.affected_minicubes[list_size] = glm::ivec3(x, y, z);
					list_size++;
				}
				else if ((tmp_drag_vector.y != 0 && norm_plane_normal.x != 0)
					&& (norm_pos_current_cube.z == norm_pos_hit_cube.z))
				{
					m_current_dragging_session.affected_minicubes[list_size] = glm::ivec3(x, y, z);
					list_size++;
				}
				else if ((tmp_drag_vector.y != 0 && norm_plane_normal.z != 0)
					&& (norm_pos_current_cube.x == norm_pos_hit_cube.x))
				{
					m_current_dragging_session.affected_minicubes[list_size] = glm::ivec3(x, y, z);
					list_size++;
				}
				else if ((tmp_drag_vector.z != 0 && norm_plane_normal.x != 0)
					&& (norm_pos_current_cube.y == norm_pos_hit_cube.y))
				{
					m_current_dragging_session.affected_minicubes[list_size] = glm::ivec3(x, y, z);
					list_size++;
				}
				else if ((tmp_drag_vector.z != 0 && norm_plane_normal.y != 0)
					&& (norm_pos_current_cube.x == norm_pos_hit_cube.x))
				{
					m_current_dragging_session.affected_minicubes[list_size] = glm::ivec3(x, y, z);
					list_size++;
				}
			}
		}
	}
}

/*This is a helper-method for the update-method. 
 It collects data from the current finger drag so that the cube can animate accordingly.*/
void Rubiks_Cube_Game_Object::contribute_to_drag_session()
{
	// shoot a ray against the dragging plane
	Camera_Ray_Cast ray_cast = Camera_Ray_Cast(m_camera, glm::vec2(g_input_state.current_cursor_pos_x, g_input_state.current_cursor_pos_y), m_current_dragging_session.plane);
	Ray_Cast_Hit_Info hit_info = ray_cast.get_hit_info();
	m_current_dragging_session.current_point = hit_info.hit_points[0];
	
	// determine the drag vector
	m_current_dragging_session.unfiltered_drag_vector = glm::vec3(m_current_dragging_session.current_point - m_current_dragging_session.starting_point);
	m_current_dragging_session.filtered_drag_vector = filter_drag_vector(m_current_dragging_session.unfiltered_drag_vector);
	
	// determine the drag axis (the axis of rotation for the affected minicubes)
	m_current_dragging_session.drag_axis = glm::normalize(glm::cross(m_current_dragging_session.plane.second, glm::normalize(m_current_dragging_session.filtered_drag_vector)));

	determine_affected_minicubes();

	// reset the drag info of all the minicubes
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int z = 0; z < 3; z++)
			{
				m_minicubes[x][y][z].set_drag_vector_length(0);
				m_minicubes[x][y][z].set_drag_axis(glm::vec3(0));
			}
		}
	}

	// set the drag info of all affected minicubes
	for (int i = 0; i < 9; i++)
	{
		glm::ivec3 index = m_current_dragging_session.affected_minicubes[i];
		m_minicubes[index.x][index.y][index.z].set_drag_vector_length(glm::length(m_current_dragging_session.filtered_drag_vector));
		m_minicubes[index.x][index.y][index.z].set_drag_axis(m_current_dragging_session.drag_axis);
	}

	// debug outputs
	char buffer[64];
	ImGui::Begin("Drag-Sessions");

	ImGui::Text("Unfiltered Drag-Vector: ");

	ImGui::SameLine();
	snprintf(buffer, sizeof buffer, "%f, %f, %f", m_current_dragging_session.unfiltered_drag_vector.x, m_current_dragging_session.unfiltered_drag_vector.y, m_current_dragging_session.unfiltered_drag_vector.z);
	ImGui::Text(buffer);

	ImGui::Text("Filtered Drag-Vector: ");

	ImGui::SameLine();
	snprintf(buffer, sizeof buffer, "%f, %f, %f", m_current_dragging_session.filtered_drag_vector.x, m_current_dragging_session.filtered_drag_vector.y, m_current_dragging_session.filtered_drag_vector.z);
	ImGui::Text(buffer);

	ImGui::Text("Drag-Vector-Length: ");

	ImGui::SameLine();
	snprintf(buffer, sizeof buffer, "%f", glm::length(m_current_dragging_session.filtered_drag_vector));
	ImGui::Text(buffer);

	ImGui::Text("Drag-Axis: ");

	ImGui::SameLine();
	snprintf(buffer, sizeof buffer, "%f, %f, %f", m_current_dragging_session.drag_axis.x, m_current_dragging_session.drag_axis.y, m_current_dragging_session.drag_axis.z);
	ImGui::Text(buffer);

	ImGui::End();
}
/*This is a helper-method for the update-method. It will be only called once 
for every dragging session (i.e. left mb down) as soon as the left mb is let go.*/
void Rubiks_Cube_Game_Object::finish_drag_session()
{
	// reset the drag info of each minicube
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int z = 0; z < 3; z++)
			{
				m_minicubes[x][y][z].set_drag_vector_length(0);
				m_minicubes[x][y][z].set_drag_axis(glm::vec3(0));
			}
		}
	}
	
	// determine, whether the filtered_drag_vector-length is over 45°
	if(glm::length(m_current_dragging_session.filtered_drag_vector) > M_PI / 4)
	{
		// rotate the model matrix of the affected minicubes
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), m_current_dragging_session.drag_axis);
		for(int i = 0; i < 9; i++)
		{
			glm::ivec3 index = m_current_dragging_session.affected_minicubes[i];
			m_minicubes[index.x][index.y][index.z].get_model_matrix() = rotation * m_minicubes[index.x][index.y][index.z].get_model_matrix();
		}
	}

	// reset the current dragging information
	m_current_dragging_session = {};
	m_finished_drg_sessions++;
	m_drg_session_live = false;
}
/*This method takes the drag vector and creates a new vec3. 
It then checks for the largest component of the drag vector and passes this to the new vec3 and returns. 
Basically every component that is less than any other component is set to 0.*/
glm::vec3 Rubiks_Cube_Game_Object::filter_drag_vector(glm::vec3 p_unfiltered_drag_vector)
{
	glm::vec3 filtered = p_unfiltered_drag_vector;

	float maximum = glm::max(glm::max(glm::abs(filtered.x), glm::abs(filtered.y)), glm::abs(filtered.z));

	if (maximum == glm::abs(filtered.x))
	{
		filtered.y = 0;
		filtered.z = 0;
	}
	else if (maximum == glm::abs(filtered.y))
	{
		filtered.x = 0;
		filtered.z = 0;
	}
	else if (maximum == glm::abs(filtered.z))
	{
		filtered.x = 0;
		filtered.y = 0;
	}

	return filtered;
}

/*This method calls the render-method of every Mini_Cube and every axis renderer.*/
void Rubiks_Cube_Game_Object::render(double p_delta_time)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				m_minicubes[i][j][k].render(m_projection, m_view);

	m_x_axis_renderer.render(p_delta_time);
	m_y_axis_renderer.render(p_delta_time);
	m_z_axis_renderer.render(p_delta_time);
}

/*This is a debug toggle method, which swaps the view of the cube from wireframe to fill 
or the other way around.*/
void Rubiks_Cube_Game_Object::toggle_wireframe_fill()
{
	if (m_toggle_wf_f)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		m_toggle_wf_f = false;
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		m_toggle_wf_f = true;
	}
}

/*This is a helper method to compile the shader text files.*/
void Rubiks_Cube_Game_Object::compile_shader(const char* p_filename, unsigned int& p_shader_id, unsigned int p_shader_type)
{
	std::ifstream file(p_filename);
	std::string line;
	std::stringstream lines;

	while (std::getline(file, line))
		lines << line << '\n';

	std::string file_string = lines.str();

	const char* file_c_string = file_string.c_str();

	p_shader_id = glCreateShader(p_shader_type);
	glShaderSource(p_shader_id, 1, &file_c_string, nullptr);
	glCompileShader(p_shader_id);

	int result;
	glGetShaderiv(p_shader_id, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		int length;
		glGetShaderiv(p_shader_id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char));
		glGetShaderInfoLog(p_shader_id, length, &length, message);
		std::cout << "SHADER COMPILE FAILURE: " << message << std::endl;
	}
}
/*This is a helper method to rotate a vec3 around another vec3.*/
void Rubiks_Cube_Game_Object::rotate_vec3(glm::vec3& p_vector, glm::vec3 p_rotation_axis, float p_angle_in_deg)
{
	glm::fquat quat = glm::fquat(0.0f, p_vector.x, p_vector.y, p_vector.z);
	quat = glm::rotate(quat, glm::radians(p_angle_in_deg), p_rotation_axis);

	p_vector.x = quat.x;
	p_vector.y = quat.y;
	p_vector.z = quat.z;
}

/*This is a helper method to project a vec3 onto another vec3.*/
void Rubiks_Cube_Game_Object::project(glm::vec3& p_target, glm::vec3 p_normal)
{
	const float n = glm::dot(p_target, p_normal);
	p_target = p_target - (n * p_normal);
	p_target = glm::normalize(p_target);
}

/*This is a helper method to gram-schmidt-orthonormalize a vec3.*/
void Rubiks_Cube_Game_Object::orthonormalize(glm::vec3& p_target, glm::vec3 p_normal_1, glm::vec3 p_normal_2)
{
	glm::vec3 target_copy = p_target;
	p_target = glm::cross(p_normal_1, p_normal_2);

	if(glm::dot(p_target, target_copy) < 0)
		p_target = -p_target;
}
	
}