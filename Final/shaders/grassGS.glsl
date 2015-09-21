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

void main() {
    vec3 Pos = gl_in[0].gl_Position.xyz;
    vec2 vPathCoord = vec2(Pos.x/fMaxTextureV, Pos.z/fMaxTextureU);
    vec4 vPathIntensity = texture(gPathMap, vPathCoord);
    vec2 pathTex = vec2(vPathCoord.x, vPathCoord.y);
    vec4 vPathColor = texture(gColorMap, pathTex);
    float fscale = vPathIntensity.x;

    if(Pos.y / renderScale.y <= 0.6 && Pos.y / renderScale.y >= 0.35 ){
        vec3 toCamera = normalize(gCameraPos - Pos);
        vec3 up = vec3(0.0,1.0,0.0);
        vec3 right = cross(toCamera, up);
        vec3 original = Pos;

        float green = abs(cos(original.y)) / 5.0;

        Pos -= (right * 6);
        gl_Position = gVP * vec4(Pos, 1.0);
        TexCoord = vec2(0.0, 0.0);
        Color = vec3(0.33 , .42+green , 0.18);
        EmitVertex();

        green = abs(cos(original.y)) / 5.0;

        Pos.y += 15;
        Pos.x += abs(5* cos(time+sin(original.x) + sin(original.z)));
        Pos.z += abs(2* sin(time+ cos(original.z)));
        Pos += (right *3);
        gl_Position = gVP * vec4(Pos, 1.0);
        TexCoord = vec2(0.0, 1.0);
        Color = vec3(0.0, .39+green, 0.0);
        EmitVertex();

        Pos.y -= 15;
        Pos.x -= abs(5* cos(time+sin(original.x) + sin(original.z)));
        Pos.z -= abs(2* sin(time+ cos(original.z)));
        gl_Position = gVP * vec4(Pos, 1.0);
        TexCoord = vec2(1.0, 0.0);
        Color = vec3(0.42, .56+green, .14);
        EmitVertex();

        green = abs(cos(original.y))/5;

        Pos.y += 15;
        Pos.x += abs(5* cos(time+sin(original.x) + sin(original.z)));
        Pos.z += abs(2* sin(time+ cos(original.z)));
        Pos += (right * 3);
        gl_Position = gVP * vec4(Pos, 1.0);
        TexCoord = vec2(1.0, 1.0);
        Color = vec3(0, .39+ green,0.0);
        EmitVertex();

        EndPrimitive();
    }

}