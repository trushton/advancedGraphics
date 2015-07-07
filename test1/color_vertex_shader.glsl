#version 330

in vec3 v_position;
in vec3 v_color;
out vec3 color;
uniform mat4 mvpMatrix;

void main() {
   gl_Position = mvpMatrix * vec4(v_position, 1.0);
   color = v_color;
}