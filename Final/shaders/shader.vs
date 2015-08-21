#version 410

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 color;

out vec3 f_color; 

uniform mat4 gWorld;

void main()
{
    gl_Position = gWorld * vec4(Position, 1.0);
    f_color = color;
}
