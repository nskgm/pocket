#version 410

in vec3 pos;

uniform vec2 a1;
uniform vec3 b1;
uniform vec3 c1;

layout(std140)
uniform ublock
{
	vec2 tex[4];
	vec4 col;
	row_major mat4 mm;
};
uniform ublock2
{
	float fa[4];
	vec4 coord;
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
	vec4 a = vec4(fa[0], fa[1], fa[2], fa[3])+coord;
	gl_Position = vec4(a1+col.xx+c1.zz, b1.xz)*tex[0].xxxx+vec4(routine(), 0.0)*mm+a;
}
