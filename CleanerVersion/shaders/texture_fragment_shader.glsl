#version 410

in vec2 tx;
out vec4 FragColor;
uniform sampler2D gSampler; 

void main(void){
   FragColor = texture(gSampler, tx.xy);
}
