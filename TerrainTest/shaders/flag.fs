#version 330

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

layout (location = 0) out vec3 WorldPosOut;
layout (location = 1) out vec3 DiffuseOut;
layout (location = 2) out vec3 NormalOut;
layout (location = 3) out vec3 TexCoordOut;

uniform sampler2D gColorMap;
vec3 original = WorldPos0;

void main()
{
	WorldPosOut     = WorldPos0;
	DiffuseOut      = vec3(normalize(WorldPos0.y * 0.10), normalize(WorldPos0.x*0.2), normalize(WorldPos0.y*0.87));
	NormalOut       = normalize(Normal0);
	TexCoordOut     = vec3(TexCoord0, 0.0);
}