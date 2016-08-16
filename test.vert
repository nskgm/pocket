#version 410

in vec3 pos;

uniform vec2 a1;
uniform vec3 b1;

vec4 test()
{
	return vec4(1.0);
}

void main()
{
	gl_Position = vec4(a1, b1.xz);
}