#version 330 

smooth in vec2 vTexCoord; 
smooth in vec3 vNormal; 
smooth in vec3 vWorldPos; 
smooth in vec4 vEyeSpacePos;

uniform sampler2D gSampler[6];
uniform sampler2D shadowMap; 

uniform vec4 vColor; 

uniform float fRenderHeight; 
uniform float fMaxTextureU; 
uniform float fMaxTextureV;

struct BaseLight
{
  vec3 Color;
  float AmbientIntensity;
  float DiffuseIntensity;
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

uniform PointLight sunLight;



layout (location = 0) out vec3 WorldPosOut;
layout (location = 1) out vec3 DiffuseOut;
layout (location = 2) out vec3 NormalOut;
layout (location = 3) out vec3 TexCoordOut;


void main() 
{

  vec3 vNormalized = normalize(vNormal);
  vec4 vTexColor = vec4(0.0);
  float fScale = vWorldPos.y/fRenderHeight;

  const float fRange1 = 0.3f;
  const float fRange2 = 0.35f;
  const float fRange3 = 0.4f;
  const float fRange4 = 0.60f;
  const float fRange5 = 0.85f;


  if(fScale >= 0.0 && fScale <= fRange1)
  {
    vTexColor = texture(gSampler[0], vTexCoord); 
  }
  else if(fScale <= fRange2) 
  { 
    fScale -= fRange1; 
    fScale /= (fRange2-fRange1); 

    float fScale2 = fScale; 
    fScale = 1.0-fScale;  

    vTexColor += texture(gSampler[0], vTexCoord)*fScale; 
    vTexColor += texture(gSampler[1], vTexCoord)*fScale2; 
  }
  else if(fScale <= fRange3)
  {
    vTexColor = texture(gSampler[1], vTexCoord);
  }
  else if(fScale <= fRange4) 
  { 
    fScale -= fRange3; 
    fScale /= (fRange4-fRange3); 

    float fScale2 = fScale; 
    fScale = 1.0-fScale;  

    vTexColor += texture(gSampler[1], vTexCoord)*fScale; 
    vTexColor += texture(gSampler[2], vTexCoord)*fScale2;       
  }
  else if(fScale <= fRange5)
    {
      fScale -= fRange4;
      fScale /= (fRange5-fRange4);

      float fScale2 = fScale;
      fScale = 1.0-fScale;

      vTexColor += texture(gSampler[2], vTexCoord)*fScale;
      vTexColor += texture(gSampler[3], vTexCoord)*fScale2;
    }
  else{
      vTexColor = texture(gSampler[3], vTexCoord);
  }

  vec2 vPathCoord = vec2((vTexCoord.x)/fMaxTextureU, (vTexCoord.y)/fMaxTextureV);
  vec4 vPathIntensity = texture(gSampler[5], vPathCoord);
  fScale = vPathIntensity.x; 

  vec4 vPathColor = texture(gSampler[4], vTexCoord);
  vec4 vFinalTexColor = fScale * vTexColor + (1-fScale) * vPathColor;

  vec4 vMixedColor = vFinalTexColor * vColor; 


  WorldPosOut = vWorldPos;
  DiffuseOut = vMixedColor.xyz;
  NormalOut = vNormalized;
  TexCoordOut = vTexCoord.xyy;
}        
