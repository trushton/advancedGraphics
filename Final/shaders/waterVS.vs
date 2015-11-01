#version 330
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 gWVP;
uniform mat4 gWorld;
uniform float time;

uniform float waveTime;
uniform float waveWidth;
uniform float waveHeight;

out vec3 Normal0;
out vec2 TexCoord0;
out vec3 WorldPos0;

float waveheight = 1.0f;
int numWaves;

const float pi = 3.14159;


void main() {


	vec4 pos = vec4(Position, 1.0);
	pos.y += ((cos(waveWidth * pos.x + waveTime) * sin(waveWidth * pos.z + waveTime)) * waveHeight) ;

    Normal0 = (gWorld * vec4(Normal, 0.0)).xyz;
    TexCoord0 = TexCoord;

    gl_Position = (gWVP * pos);
    WorldPos0 = (gWorld * vec4(Position, 1.0)).xyz;

}