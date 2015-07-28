#version 330

layout (points) in;
layout(triangle_strip) out;
layout(max_vertices = 3) out;

uniform mat4 gVP;
uniform vec3 gCameraPos;
uniform float time;

out vec3 Color;

void main() {
    vec3 Pos = gl_in[0].gl_Position.xyz;
    vec3 toCamera = normalize(gCameraPos - Pos);
    vec3 up = vec3(0.0,1.0,0.0);
    vec3 right = cross(toCamera, up);
    vec3 original = Pos;

    Pos -= (right * 6);
    gl_Position = gVP * vec4(Pos, 1.0);
    Color = vec3(0.0, 0.0, 1.0);
    EmitVertex();

    Pos += (right * 6);
    gl_Position = gVP * vec4(Pos, 1.0);
    Color = vec3(0.0, 0.2, 0.0);
    EmitVertex();

    Pos.y += abs(40* cos(time+sin(original.x) + sin(original.z)));
    Pos -= (right * 3);
    gl_Position = gVP * vec4(Pos, 1.0);
    Color = vec3(1.0, 3.0, 1.0);
    EmitVertex();

    EndPrimitive();
}