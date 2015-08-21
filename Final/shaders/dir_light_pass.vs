#version 330

layout (location = 0) in vec3 Position
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

out VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec3 TexCoords;
} vs_out

uniform mat4 gWVP;
uniform Mat4 gWorld;

void main(){
    gl_Position = gWVP * vec4(Position, 1.0);

    vs_out.FragPos = vec3(gWorld * vec4(Position, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * normal;
    vs_out.TexCoords = TexCoord;
}