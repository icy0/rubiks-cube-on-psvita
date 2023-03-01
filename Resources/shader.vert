#version 330 core

// vertex attributes
layout(location = 0) in vec4 a_position;
layout(location = 1) in vec2 a_texture_coords;
layout(location = 2) in vec3 a_normal;

// output to fragment shader
out vertex_fragment_interface
{
	vec2 texture_coords;
	vec3 normal;
};

// uniform from application
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform float u_drag_vector_length;
uniform vec3 u_drag_axis;

mat4 build_rotation_matrix(vec3 axis, float angle)
{
	angle = clamp(angle, 0, 1.57079); // clamp to 90° (== PI / 2)
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
	mat4 rotation = mat4(1);

	if(u_drag_vector_length > 0)
	{
		rotation = build_rotation_matrix(-u_drag_axis, u_drag_vector_length);
	}

	gl_Position =  u_projection * u_view * rotation * u_model * a_position;

	texture_coords = a_texture_coords;
	normal = vec3(a_normal.x, a_normal.y, a_normal.z);
}