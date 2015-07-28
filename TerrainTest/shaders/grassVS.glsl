#version 330
layout (location = 0) in vec3 Position;

uniform mat4 model;

void main() {

    gl_Position = model * vec4(Position.x, Position.y, Position.z, 1.0);
}