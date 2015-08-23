#version 410

layout (location = 0) in vec3 Position;
layout (location = 4) in vec3 Color;

out VS_OUT {
    vec3 color_value;
} vs_out;

void main()                                                                         
{                                                                                   
    gl_Position = vec4(Position, 1.0);
    vs_out.color_value = Color;
}                                                                                   
