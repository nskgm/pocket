#version 410

// input
layout (location=0) in vec3 vertex_position;
layout (location=1) in vec4 vertex_color;

// ouptut
out vec4 color;

uniform ublock
{
	mat4 world;
	mat4 lookat;
	mat4 perspective;
};

void main()
{
	vec4 pos = world * vec4(vertex_position, 1.0);
	pos = lookat * pos;
	gl_Position = perspective * pos;
	color = vertex_color;
}
