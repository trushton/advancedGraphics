#include "ds_spot_light_pass_tech.h"

DSSpotLightPassTech::DSSpotLightPassTech()
{
}

void DSSpotLightPassTech::init()
{

    // load and compile the shaders
    loadShaders();

    //  link all shaders together
    initShaderProgram();

    // get the variables from the shaders
    initShaderLocations();

    // create the VAO
    glUseProgram(program);
}

void DSSpotLightPassTech::enable()
{
    glUseProgram(program);
}

void DSSpotLightPassTech::bind()
{

}

void DSSpotLightPassTech::unbind()
{

}

void DSSpotLightPassTech::loadShaders()
{
    shaders[0] = loadShader("../shaders/light_pass.vs", GL_VERTEX_SHADER);
    shaders[1] = loadShader("../shaders/point_light_pass.fs", GL_FRAGMENT_SHADER);
}

void DSSpotLightPassTech::initShaderProgram()
{
    program = glCreateProgram();
    glAttachShader(program, shaders[0]);
    glAttachShader(program, shaders[1]);

    glLinkProgram(program);

    GLint shader_status;
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if (!shader_status)
    {
        std::cerr << "Unable to create shader program!" << std::endl;

        char buffer[512];
        glGetProgramInfoLog(program, 512, NULL, buffer); // inserts the error into the buffer
        std::cerr << buffer << std::endl;

        exit(1);
    }
}

void DSSpotLightPassTech::initShaderLocations()
{

    glUseProgram(program);
    locations["gWVP"] = glGetUniformLocation(program, "gWVP");
    locations["gPositionMap"] = glGetUniformLocation(program,"gPositionMap");
    locations["gColorMap"] = glGetUniformLocation(program,"gColorMap");
    locations["gNormalMap"] = glGetUniformLocation(program,"gNormalMap");
    locations["gEyeWorldPos"] = glGetUniformLocation(program,"gEyeWorldPos");
    locations["gMatSpecularIntensity"] = glGetUniformLocation(program,"gMatSpecularIntensity");
    locations["gSpecularPower"] = glGetUniformLocation(program,"gSpecularPower");
    locations["gScreenSize"] = glGetUniformLocation(program,"gScreenSize");



    locations["gSpotLight.pointBase.Base.Color"] = glGetUniformLocation(program, "gSpotLight.pointBase.Base.Color");
    locations["gSpotLight.pointBase.Base.AmbientIntensity"] = glGetUniformLocation(program, "gSpotLight.pointBase.Base.AmbientIntensity");
    locations["gSpotLight.pointBase.Position"] = glGetUniformLocation(program, "gSpotLight.pointBase.Position");
    locations["gSpotLight.pointBase.Base.DiffuseIntensity"] = glGetUniformLocation(program, "gSpotLight.pointBase.Base.DiffuseIntensity");
    locations["gSpotLight.pointBase.Atten.Constant"] = glGetUniformLocation(program, "gSpotLight.pointBase.Atten.Constant");
    locations["gSpotLight.pointBase.Atten.Linear"] = glGetUniformLocation(program, "gSpotLight.pointBase.Atten.Linear");
    locations["gSpotLight.pointBase.Atten.Exp"] = glGetUniformLocation(program, "gSpotLight.pointBase.Atten.Exp");

    locations["gSpotLight.Direction"] = glGetUniformLocation(program, "gSpotLight.Direction");
    locations["gSpotLight.Cutoff"] = glGetUniformLocation(program, "gSpotLight.Cutoff");
}

void DSSpotLightPassTech::SetSpotLight(const SpotLight &Light)
{
    glUniform3f(locations["gSpotLight.pointBase.Base.Color"], Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(locations["gSpotLight.pointBase.Base.AmbientIntensity"], Light.AmbientIntensity);
    glUniform1f(locations["gSpotLight.pointBase.Base.DiffuseIntensity"], Light.DiffuseIntensity);
    glUniform3f(locations["gSpotLight.pointBase.Position"], Light.Position.x, Light.Position.y, Light.Position.z);
    glUniform1f(locations["gSpotLight.pointBase.Atten.Constant"], Light.Attenuation.Constant);
    glUniform1f(locations["gSpotLight.pointBase.Atten.Linear"], Light.Attenuation.Linear);
    glUniform1f(locations["gSpotLight.pointBase.Atten.Exp"], Light.Attenuation.Exp);

    glUniform3f(locations["gSpotLight.Direction"], Light.Direction.x, Light.Direction.y, Light.Direction.z);
    glUniform1f(locations["gSpotLight.Cutoff"], Light.Cutoff);


}


