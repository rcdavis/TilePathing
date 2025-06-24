// Colored Tile Vertex Shader
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;
uniform vec4 u_Color;

struct VertexOutput
{
	vec4 color;
};

layout(location = 0) out VertexOutput OutVert;

void main()
{
	OutVert.color = u_Color;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}
