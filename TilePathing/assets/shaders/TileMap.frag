// Tile Map Fragment Shader
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

struct VertexOutput
{
	vec2 texCoord;
};

in VertexOutput Output;

layout(binding = 0) uniform sampler2D u_Texture;
uniform int u_EntityId;

void main()
{
	color = texture(u_Texture, Output.texCoord);

	color2 = u_EntityId;
}
