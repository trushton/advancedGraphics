#version 410

in vec4 clipSpace;

out vec4 out_Color;

uniform sampler2D reflectionTexture;

void main(void) {

	vec2 ndc = (clipSpace.xy/clipSpace.w) / 2.0 + 0.5;

	out_Color = texture(reflectionTexture, vec2(ndc.x, -ndc.y));

}