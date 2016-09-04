#version 410

// input
in vec4 color;

// output
out vec4 frag_color;

void main()
{
	// 頂点からの色をそのまま
	frag_color = color;
}
