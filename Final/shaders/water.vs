#version 410

layout (location = 0) in vec2 position;

out vec4 clipSpace;

uniform mat4 mvp;

void main(void) {

	clipSpace = mvp * vec4(position.x, 0.0, position.y, 1.0);
	gl_Position = clipSpace;

}