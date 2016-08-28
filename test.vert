#version 410

in vec3 pos;

uniform vec2 a1;
uniform vec3 b1;
uniform vec3 c1;

layout(std140)
uniform vert_uniforms
{
	vec2 tex;
	vec4 col;
};

subroutine vec3 uniform_routine();
subroutine uniform uniform_routine routine;

subroutine(uniform_routine)
vec3 r1()
{
	return vec3(1.0);
}
subroutine(uniform_routine)
vec3 r2()
{
	return vec3(0.0);
}

vec4 test()
{
	return vec4(1.0);
}

void main()
{
	gl_Position = vec4(a1+col.xx+c1.zz, b1.xz)*tex.xxxx+vec4(routine(), 0.0);
}
