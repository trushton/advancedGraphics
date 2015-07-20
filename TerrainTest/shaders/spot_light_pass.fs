#version 330

vec3 WorldPos;

struct BaseLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct DirectionalLight
{
    BaseLight Base;
    vec3 Direction;
};

struct Attenuation
{
    float Constant;
    float Linear;
    float Exp;
};

struct PointLight
{
    BaseLight Base;
    vec3 Position;
    Attenuation Atten;
};

struct SpotLight
{
    PointLight pointBase;
    vec3 Direction;
    float Cutoff;
};

uniform sampler2D gPositionMap;
uniform sampler2D gColorMap;
uniform sampler2D gNormalMap;
uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLight;
uniform SpotLight gSpotLight;
uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;
uniform int gLightType;
uniform vec2 gScreenSize;

vec4 CalcLightInternal(BaseLight Light,
                       vec3 LightDirection,
                       vec3 WorldPos,
                       vec3 Normal)
{
    vec4 AmbientColor = vec4(Light.Color * Light.AmbientIntensity, 1.0);
    float DiffuseFactor = dot(Normal, -LightDirection);

    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0.0) {
        DiffuseColor = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor, 1.0);

        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos);
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);        
        if (SpecularFactor > 0.0) {
            SpecularFactor = pow(SpecularFactor, gSpecularPower);
            SpecularColor = vec4(Light.Color * gMatSpecularIntensity * SpecularFactor, 1.0);
        }
    }

    return (AmbientColor + DiffuseColor + SpecularColor);
}

vec4 CalcDirectionalLight(vec3 WorldPos, vec3 Normal)
{
    return CalcLightInternal(gDirectionalLight.Base,
                             gDirectionalLight.Direction,
                             WorldPos,
                             Normal);
}

vec4 CalcPointLight(struct PointLight p, vec3 Normal)
{
    vec3 LightDirection = WorldPos - p.Position;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);

    vec4 Color = CalcLightInternal(p.Base, LightDirection, WorldPos, Normal);

    float attenuation =  p.Atten.Constant +
                         p.Atten.Linear * Distance +
                         p.Atten.Exp * Distance * Distance;

    attenuation = max(1.0, attenuation);

    return Color / attenuation;
}

vec4 CalcSpotLight(struct SpotLight l, vec3 Normal)
{
    vec3 LightToPixel = normalize((WorldPos - l.pointBase.Position));
    float SpotFactor = dot(LightToPixel, l.Direction);

    if (SpotFactor > l.Cutoff) {
        vec4 Color = CalcPointLight(l.pointBase, Normal);
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));
    }
    else {
        return vec4(0,0,0,0);
    }
}


vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / gScreenSize;
}



out vec4 FragColor;



void main()
{
    vec2 TexCoord = CalcTexCoord();
    WorldPos = texture(gPositionMap, TexCoord).xyz;
    vec3 Color = texture(gColorMap, TexCoord).xyz;
    vec3 Normal = texture(gNormalMap, TexCoord).xyz;
    Normal = normalize(Normal);

    FragColor = vec4(Color, 1.0) * CalcSpotLight(gSpotLight, Normal);
}
