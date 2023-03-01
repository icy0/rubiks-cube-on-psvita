#version 330 core

// input from vertex shader
in vertex_fragment_interface
{
	vec2 texture_coords;
	vec3 normal;
};

// output to framebuffer
out vec4 color;

// texture from application
uniform sampler2D u_basecolor_texture;
uniform int u_hitflag;

void main()
{
	color = texture(u_basecolor_texture, texture_coords);

	if(u_hitflag != 0)
	{
		// color = vec4(1.0, 0.0, 1.0, 1.0);
	}
}

