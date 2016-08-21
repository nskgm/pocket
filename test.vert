#version 410

in vec3 pos;

uniform vec2 a1;
uniform vec3 b1;
uniform vec3 c1;

uniform vert_uniforms
{
	mat4 pvw;
	vec4 col;
};

vec4 test()
{
	return vec4(1.0);
}

void main()
{
	gl_Position = vec4(a1+col.xx+c1.zz, b1.xz)*pvw;
}