// Colored Tile Fragment Shader
#version 450 core

layout(location = 0) out vec4 color;

struct VertexOutput
{
	vec4 color;
};

layout(location = 0) in VertexOutput InVert;

void main()
{
	if (InVert.color.a < 0.1)
		discard;
	color = InVert.color;
}
