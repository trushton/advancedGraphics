#version 330

in vec3 color;

void main() {
    gl_FragColor = vec4(color.rgb, 1.0);
}