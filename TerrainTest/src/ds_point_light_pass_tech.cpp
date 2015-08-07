#include "ds_point_light_pass_tech.h"

DSPointLightPassTech::DSPointLightPassTech()
{
}

void DSPointLightPassTech::init()
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

void DSPointLightPassTech::enable()
{
    glUseProgram(program);
}

void DSPointLightPassTech::bind()
{

}

void DSPointLightPassTech::unbind()
{

}

void DSPointLightPassTech::loadShaders()
{
    shaders[0] = loadShader("../shaders/light_pass.vs", GL_VERTEX_SHADER);
    shaders[1] = loadShader("../shaders/point_light_pass.fs", GL_FRAGMENT_SHADER);
}

void DSPointLightPassTech::initShaderProgram()
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

void DSPointLightPassTech::initShaderLocations()
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


    locations["gPointLight.Base.Color"] = glGetUniformLocation(program, "gPointLight.Base.Color");
    locations["gPointLight.Base.AmbientIntensity"] = glGetUniformLocation(program,
                                                                          "gPointLight.Base.AmbientIntensity");
    locations["gPointLight.Position"] = glGetUniformLocation(program, "gPointLight.Position");
    locations["gPointLight.Base.DiffuseIntensity"] = glGetUniformLocation(program,
                                                                          "gPointLight.Base.DiffuseIntensity");
    locations["gPointLight.Atten.Constant"] = glGetUniformLocation(program, "gPointLight.Atten.Constant");
    locations["gPointLight.Atten.Linear"] = glGetUniformLocation(program, "gPointLight.Atten.Linear");
    locations["gPointLight.Atten.Exp"] = glGetUniformLocation(program, "gPointLight.Atten.Exp");
}

void DSPointLightPassTech::SetPointLight(const PointLight &Light)
{
    glUniform3f(locations["gPointLight.Base.Color"], Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(locations["gPointLight.Base.AmbientIntensity"], Light.AmbientIntensity);
    glUniform1f(locations["gPointLight.Base.DiffuseIntensity"], Light.DiffuseIntensity);
    glUniform3f(locations["gPointLight.Position"], Light.Position.x, Light.Position.y, Light.Position.z);
    std::cout << Light.Position.x << " " << Light.Position.y << " " << Light.Position.z << std::endl;
    glUniform1f(locations["gPointLight.Atten.Constant"], Light.Attenuation.Constant);
    glUniform1f(locations["gPointLight.Atten.Linear"], Light.Attenuation.Linear);
    glUniform1f(locations["gPointLight.Atten.Exp"], Light.Attenuation.Exp);
}


