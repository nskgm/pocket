#version 410

// input
layout (location=0) in vec2 vertex_position;
layout (location=1) in vec4 vertex_color;

// ouptut
out vec4 color;

uniform ublock
{
	mat4 world;
	mat4 ortho;
};

void main()
{
	vec4 pos = world * vec4(vertex_position, 0.0, 1.0);
	gl_Position = ortho * pos;
	color = vertex_color;
}
