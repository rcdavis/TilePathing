// Tile Map Fragment Shader
#version 460 core

layout(location = 0) in vec2 v_TexCoord;

layout(binding = 0) uniform sampler2D u_Texture;
uniform vec4 u_Color;

layout(location = 0) out vec4 color;

void main() {
	vec4 finalColor = texture(u_Texture, v_TexCoord) * u_Color;
	if (finalColor.a < 0.1)
		discard;
	color = finalColor;
}
