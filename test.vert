in vec3 pos;

uniform vec2 a1;
uniform vec3 b1;

void main()
{
	gl_Position = vec4(a1, b1.xz);
}