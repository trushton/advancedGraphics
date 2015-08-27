#version 330

uniform float waveTime;
uniform float waveWidth;
uniform float waveHeight;

uniform mat4 gWVP;
uniform mat4 gWorld;

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;                                             
layout (location = 2) in vec3 Normal;

out vec3 Normal0;
out vec2 TexCoord0;
out vec3 WorldPos0;

void main(void)
{
    vec4 v = vec4(Position,1.0);
    v.y += cos(waveWidth * (v.z) + waveTime) * waveHeight * TexCoord.x;

    gl_Position = gWVP * v;
    TexCoord0 = TexCoord;

    WorldPos0 = (gWorld * vec4(Position,1.0)).xyz;
    Normal0 = (gWorld * vec4(Normal, 0.0)).xyz;

}