#version 330

layout (points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

uniform mat4 gVP;
uniform vec3 gCameraPos;
uniform float time;
uniform vec3 renderScale;
uniform sampler2D gPathMap;
uniform sampler2D gColorMap;
uniform float fMaxTextureU;
uniform float fMaxTextureV;

out vec3 Color;
out vec2 TexCoord;
out vec4 outPos;

void main() {
    vec3 Pos = gl_in[0].gl_Position.xyz;



    if(Pos.y / renderScale.y <= 0.6 && Pos.y / renderScale.y >= 0.30){
        vec3 toCamera = normalize(gCameraPos - Pos);
        vec3 up = vec3(0.0,1.0,0.0);
        vec3 right = cross(toCamera, up);
        vec3 original = Pos;

        float green = abs(cos(original.y)) / 5.0;

        Pos -= (right * 6);
        gl_Position = gVP * vec4(Pos, 1.0);
        TexCoord = vec2(0.0, 0.0);
        outPos = gl_Position;
        EmitVertex();

        green = abs(cos(original.y)) / 5.0;

        Pos.y += 10;
        Pos.x += abs(5* cos(time+sin(original.x) + sin(original.z)));
        Pos.z += abs(2* sin(time+ cos(original.z)));
        Pos += (right *3);
        gl_Position = gVP * vec4(Pos, 1.0);
        TexCoord = vec2(0.0, 1.0);
                outPos = gl_Position;

        EmitVertex();

        Pos.y -= 10;
        Pos.x -= abs(5* cos(time+sin(original.x) + sin(original.z)));
        Pos.z -= abs(2* sin(time+ cos(original.z)));
        gl_Position = gVP * vec4(Pos, 1.0);
        TexCoord = vec2(1.0, 0.0);
                outPos = gl_Position;

        EmitVertex();

        green = abs(cos(original.y))/5;

        Pos.y += 10;
        Pos.x += abs(5* cos(time+sin(original.x) + sin(original.z)));
        Pos.z += abs(2* sin(time+ cos(original.z)));
        Pos += (right * 3);
        gl_Position = gVP * vec4(Pos, 1.0);
        TexCoord = vec2(1.0, 1.0);
                outPos = gl_Position;

        EmitVertex();

        EndPrimitive();
    }

}