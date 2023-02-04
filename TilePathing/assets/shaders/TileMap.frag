// Tile Map Fragment Shader
#version 450 core

layout(location = 0) out vec4 color;

struct VertexOutput
{
	vec2 texCoord;
};

layout(location = 0) in VertexOutput InVert;

layout(binding = 0) uniform sampler2D u_Texture;

void main()
{
	vec4 finalColor = texture(u_Texture, InVert.texCoord);
	if (finalColor.a < 0.1)
		discard;
	color = finalColor;
}
