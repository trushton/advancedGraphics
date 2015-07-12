#include "ds_dir_light_pass_tech.h"

DSDirLightPassTech::DSDirLightPassTech()
{
}

void DSDirLightPassTech::init()
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

void DSDirLightPassTech::enable()
{
    glUseProgram(program);
}

void DSDirLightPassTech::bind()
{

}

void DSDirLightPassTech::unbind()
{

}

void DSDirLightPassTech::loadShaders()
{
    shaders[0] = loadShader("../shaders/light_pass.vs", GL_VERTEX_SHADER);
    shaders[1] = loadShader("../shaders/dir_light_pass.fs", GL_FRAGMENT_SHADER);
}

void DSDirLightPassTech::initShaderProgram()
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

void DSDirLightPassTech::initShaderLocations()
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

    locations["gDirectionalLight.Base.Color"] = glGetUniformLocation(program, "gDirectionalLight.Base.Color");
    locations["gDirectionalLight.Base.AmbientIntensity"] = glGetUniformLocation(program,
                                                                                "gDirectionalLight.Base.AmbientIntensity");
    locations["gDirectionalLight.Direction"] = glGetUniformLocation(program, "gDirectionalLight.Direction");
    locations["gDirectionalLight.Base.DiffuseIntensity"] = glGetUniformLocation(program,
                                                                                "gDirectionalLight.Base.DiffuseIntensity");


}

void DSDirLightPassTech::SetDirectionalLight(const DirectionalLight &Light)
{
    glUniform3f(locations["gDirectionalLight.Base.Color"], Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(locations["gDirectionalLight.Base.AmbientIntensity"], Light.AmbientIntensity);
    glm::vec3 Direction = Light.Direction;
    glm::normalize(Direction);
    glUniform3f(locations["gDirectionalLight.Direction"], Direction.x, Direction.y, Direction.z);
    glUniform1f(locations["gDirectionalLight.Base.DiffuseIntensity"], Light.DiffuseIntensity);
}


