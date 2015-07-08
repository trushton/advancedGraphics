#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texture;
layout (location = 2) in vec3 v_normal;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;

out vec2 TexCoord0;
out vec3 Normal0;
out vec3 WorldPos0;

void main()
{
    gl_Position = mvpMatrix * vec4(v_position, 1.0);
    TexCoord0 = v_texture;
    Normal0 = (modelMatrix * vec4(v_normal, 0.0)).xyz;
    WorldPos0 = (modelMatrix * vec4(v_position, 1.0)).xyz;
}