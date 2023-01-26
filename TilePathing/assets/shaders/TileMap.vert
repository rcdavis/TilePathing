// Tile Map Vertex Shader
#version 450 core

in vec3 a_Position;
in vec2 a_TexCoord;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec2 texCoord;
};

out VertexOutput Output;

void main()
{
	Output.texCoord = a_TexCoord;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}
