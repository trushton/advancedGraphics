#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 v_texture;

out vec2 texture;
uniform mat4 mvpMatrix;

void main() {
    gl_Position = mvpMatrix * vec4(v_position, 1.0);
	texture = v_texture;
}