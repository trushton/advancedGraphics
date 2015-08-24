#version 330
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 gWVP;
uniform mat4 gWorld;
uniform float time;

out vec3 Normal0;
out vec2 TexCoord0;
out vec3 WorldPos0;

float wavelength[4];
float speed[4];
float amplitude[4];
vec2 direction[4];

float waveheight = 3.0f;
int numWaves;

const float pi = 3.14159;

float wave(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    return amplitude[i] * sin(theta * frequency + time * phase);
}

float waveHeight(float x, float y) {
    float height = 0.0;
    for (int i = 0; i < numWaves; ++i)
        height += wave(i, x, y);
    return height;
}

float dWavedx(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].x * frequency;
    return A * cos(theta * frequency + time * phase);
}

float dWavedy(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].y * frequency;
    return A * cos(theta * frequency + time * phase);
}

vec3 waveNormal(float x, float y) {
    float dx = 0.0;
    float dy = 0.0;
    for (int i = 0; i < numWaves; ++i) {
        dx += dWavedx(i, x, y);
        dy += dWavedy(i, x, y);
    }
    vec3 n = vec3(-dx, -dy, 1.0);
    return normalize(n);
}

void main() {
    numWaves = 4;

	wavelength[0] = 800 * pi;
	speed[0] = 1000.0f;
	amplitude[0] = 30f;
	direction[0] = vec2(0.0, 1.0);

	wavelength[1] = 500 * pi;
	speed[1] = 37.0f;
	amplitude[1] = 13.0f;
	direction[1] = vec2(1.0, 0.5);

	wavelength[2] = 223 * pi;
	speed[2] = 9.5f;
	amplitude[2] = 10.0f;
	direction[2] = vec2(-0.4, 0.6);

	wavelength[3] = 500 * pi;
	speed[3] = 60.0f;
	amplitude[3] = 10.0f;
	direction[3] = vec2(-0.4, -0.6);

	vec4 pos = vec4(Position, 1.0);
	pos.y = waveHeight(pos.x, pos.z);

    Normal0 = (gWorld * vec4(waveNormal(pos.x, pos.z), 0.0)).xyz;
    TexCoord0 = TexCoord;

    gl_Position = (gWVP * pos);
    WorldPos0 = (gWorld * vec4(Position, 1.0)).xyz;

}