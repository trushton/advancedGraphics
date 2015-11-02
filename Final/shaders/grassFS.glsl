#version 330

uniform sampler2D gColorMap;

in vec4 Pos;
in vec3 Color;
in vec2 TexCoord;

layout (location = 0) out vec3 WorldPosOut;
layout (location = 1) out vec3 DiffuseOut;

void main() {

    WorldPosOut = Pos.xyz;

    DiffuseOut = vec4(texture(gColorMap, TexCoord).xyz, 1.0).xyz;
        if(DiffuseOut.r == 0 && DiffuseOut.g == 0 && DiffuseOut.b == 0){
            discard;
        }

}