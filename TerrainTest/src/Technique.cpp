//
// Created by Christine Johnson on 7/4/15.
//

#include "Technique.h"
#include <iostream>

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

Technique::Technique()
{
}

void Technique::init()
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

void Technique::bind()
{

}

void Technique::unbind()
{

}

void Technique::enable() {
    glUseProgram(program);
}

void Technique::loadShaders()
{
    shaders[0] = loadShader("../shaders/null_technique.vs", GL_VERTEX_SHADER);
    shaders[1] = loadShader("../shaders/null_technique.fs", GL_FRAGMENT_SHADER);

}

void Technique::initShaderProgram()
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

void Technique::initShaderLocations()
{
    glUseProgram(program);
    locations["gWVP"] = glGetUniformLocation(program, "gWVP");
    if (locations["gWVP"]  == INVALID_UNIFORM_LOCATION ) {

        std::cout << "TEST!" << std::endl;
    }
    //return false;
}

