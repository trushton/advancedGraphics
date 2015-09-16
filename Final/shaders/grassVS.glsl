#version 330
layout (location = 0) in vec3 Position;

uniform mat4 model;
uniform mat4 HeightmapScaleMatrix;
uniform mat4 gVP;
uniform mat4 view;
uniform mat4 projection;
void main() {

    gl_Position = vec4(Position.x, Position.y, Position.z, 1.0);
}