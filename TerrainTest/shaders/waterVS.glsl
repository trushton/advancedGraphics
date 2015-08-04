#version 330
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;

out vec3 fcolor;
uniform mat4 mvp;
uniform float time;

void main() {
    vec3 original = Position;

    gl_Position = mvp * vec4(Position.x, Position.y + abs(50* cos(time+sin(original.x) + sin(original.z))), Position.z, 1.0);
    fcolor = Color;
}