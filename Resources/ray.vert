#version 330 core

// vertex attributes
layout(location = 0) in vec4 a_position;

// output to fragment shader
//out vertex_fragment_interface
//{
//
//};

// uniform from application
uniform mat4 u_mvp;

void main()
{
	gl_Position =  u_mvp * a_position;
}