#version 410

in vec3 f_color;

out vec4 FragColor;

void main()
{
    FragColor = vec4(f_color.rgb, 1.0);
}
