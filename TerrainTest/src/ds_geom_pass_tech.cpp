
#include <limits.h>
#include <string.h>
#include <iostream>
#include "ds_geom_pass_tech.h"

#define INVALID_UNIFORM_LOCATION 0xffffffff



DSGeomPassTech::DSGeomPassTech()
{
}

void DSGeomPassTech::init()
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

void DSGeomPassTech::bind()
{

}

void DSGeomPassTech::unbind()
{

}

void DSGeomPassTech::enable() {
    glUseProgram(program);
}

void DSGeomPassTech::loadShaders()
{
    shaders[0] = loadShader("../shaders/geometry_pass.vs", GL_VERTEX_SHADER);
    shaders[1] = loadShader("../shaders/geometry_pass.fs", GL_FRAGMENT_SHADER);

}

void DSGeomPassTech::initShaderProgram()
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

void DSGeomPassTech::initShaderLocations()
{
    glUseProgram(program);
    locations["gWVP"] = glGetUniformLocation(program, "gWVP");
    locations["gWorld"] = glGetUniformLocation(program, "gWorld");
    locations["gColorMap"] = glGetUniformLocation(program, "gColorMap");

    if (locations["gWVP"]  == INVALID_UNIFORM_LOCATION ||
        locations["gWorld"] == INVALID_UNIFORM_LOCATION || locations["gColorMap"] ==INVALID_UNIFORM_LOCATION ) {

        std::cout << "TEST!" << std::endl;
    }
    //return false;
}
