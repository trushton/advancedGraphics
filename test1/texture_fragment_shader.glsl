#version 330

in vec2 texture;
uniform sampler2D gSampler;

void main() {
   gl_FragColor = texture2D(gSampler, texture.xy);
}