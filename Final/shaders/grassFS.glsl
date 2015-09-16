#version 330

uniform sampler2D gColorMap;

in vec3 Color;
in vec2 TexCoord;
out vec4 FragColor;

void main() {
    FragColor = vec4(Color, 1.0);

}