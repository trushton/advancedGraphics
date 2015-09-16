#version 410                                                                       
                                                                                    
uniform sampler2D color_map;

in vec2 TexCoord;
in vec3 Color2;

out vec4 FragColor;                                                                 
                                                                                    
void main()                                                                         
{

    FragColor = texture(color_map, vec2(TexCoord.x, TexCoord.y)) * vec4(Color2.xyz ,1.0);
    if(FragColor.r >= 0.9 || FragColor.g >= 0.9 || FragColor.b >= 0.9){
            discard;
        }
}
