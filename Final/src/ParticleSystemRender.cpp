//
// Created by trushton on 8/22/15.
//
#include "ParticleSystemRender.h"


ParticleRenderProgram::ParticleRenderProgram()
{
    init();
}

void ParticleRenderProgram::init()
{
    loadShaders();
    initShaderProgram();
    initShaderLocations();

}


void ParticleRenderProgram::loadShaders()
{
    shaders[0] = loadShader("../shaders/particles_render.vs", GL_VERTEX_SHADER);
    shaders[1] = loadShader("../shaders/particles_render.fs", GL_FRAGMENT_SHADER);
    shaders[2] = loadShader("../shaders/particles_render.gs", GL_GEOMETRY_SHADER);
}

void ParticleRenderProgram::initShaderProgram()
{
    program = glCreateProgram();
    glAttachShader(program, shaders[0]);
    glAttachShader(program, shaders[1]);
    glAttachShader(program, shaders[2]);

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

void ParticleRenderProgram::initShaderLocations()
{
    locations["vp"] = glGetUniformLocation(program, "vp");
    locations["camera_pos"] = glGetUniformLocation(program, "camera_pos");
    locations["color_map"] = glGetUniformLocation(program, "color_map");
    locations["billboard_size"] = glGetUniformLocation(program, "billboard_size");


    for (const auto &pair : locations)
    {
        if (pair.second < 0)
        {
            std::cout << "Program: Render  Unable to find location: " << pair.first << std::endl;
        }
    }
}

void ParticleRenderProgram::enable(){
    glUseProgram(program);
}

void ParticleRenderProgram::bind(){

}

void ParticleRenderProgram::unbind(){

}

