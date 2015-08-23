//
// Created by trushton on 8/22/15.
//

#include "ParticleSystemUpdate.h"

ParticleUpdateProgram::ParticleUpdateProgram()
{
    init();
}

void ParticleUpdateProgram::init()
{
    loadShaders();
    initShaderProgram();
    initShaderLocations();
}

void ParticleUpdateProgram::loadShaders()
{
    shaders[0] = loadShader("../shaders/particles_update.vs", GL_VERTEX_SHADER);
    shaders[1] = loadShader("../shaders/particles_update.gs", GL_GEOMETRY_SHADER);
}

void ParticleUpdateProgram::initShaderProgram()
{
    program = glCreateProgram();
    glAttachShader(program, shaders[0]);
    glAttachShader(program, shaders[1]);


    const GLchar *varyings[4];
    varyings[0] = "Type1";
    varyings[1] = "Position1";
    varyings[2] = "Velocity1";
    varyings[3] = "Age1";
    varyings[4] = "Color1";


    glTransformFeedbackVaryings(program, 5, varyings, GL_INTERLEAVED_ATTRIBS);

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

void ParticleUpdateProgram::initShaderLocations()
{

    locations["dt"] = glGetUniformLocation(program, "dt");
    locations["random_texture"] = glGetUniformLocation(program, "random_texture");
    locations["time"] = glGetUniformLocation(program, "time");
    locations["launcher_lifetime"] = glGetUniformLocation(program, "launcher_lifetime");
    locations["shell_lifetime"] = glGetUniformLocation(program, "shell_lifetime");
    locations["secondary_shell_lifetime"] = glGetUniformLocation(program, "secondary_shell_lifetime");
    locations["particleColor"] = glGetUniformLocation(program, "particleColor");


    for (const auto &pair : locations)
    {
        if (pair.second < 0)
        {
            std::cout << "Program: Update  Unable to find location: " << pair.first << std::endl;
        }
    }
}

void ParticleUpdateProgram::enable(){
    glUseProgram(program);
}

void ParticleUpdateProgram::bind(){

}

void ParticleUpdateProgram::unbind(){

}
