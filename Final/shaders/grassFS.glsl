#version 330

uniform sampler2D gColorMap;

in vec3 Color;
in vec2 TexCoord;
out vec4 FragColor;

void main() {

    FragColor = vec4(texture(gColorMap, TexCoord).xyz, 1.0);
    if(FragColor.r == 0 && FragColor.g == 0 && FragColor.b == 0){
        discard;
    }
}