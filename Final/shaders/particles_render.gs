#version 410

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

uniform mat4 vp;
uniform vec3 camera_pos;
uniform float billboard_size;

in VS_OUT
{
    vec3 color_value;

} gs_in[];

out vec3 Color2;
out vec2 TexCoord;

void main()
{
    Color2 = gs_in[0].color_value;

    vec3 Pos = gl_in[0].gl_Position.xyz;
    vec3 toCamera = normalize(camera_pos - Pos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up) * billboard_size;

    Pos -= right;
    gl_Position = vp * vec4(Pos, 1.0);
    TexCoord = vec2(0.0, 0.0);
    EmitVertex();

    Pos.y += billboard_size;
    gl_Position = vp * vec4(Pos, 1.0);
    TexCoord = vec2(0.0, 1.0);
    EmitVertex();

    Pos.y -= billboard_size;
    Pos += right;
    gl_Position = vp * vec4(Pos, 1.0);
    TexCoord = vec2(1.0, 0.0);
    EmitVertex();

    Pos.y += billboard_size;
    gl_Position = vp * vec4(Pos, 1.0);
    TexCoord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
