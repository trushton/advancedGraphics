//
// Created by trushton on 7/27/15.
//

#include "billboard_tech.h"

billboard_tech::billboard_tech(){}

void billboard_tech::init(){
    //load and compile shaders
    loadShaders();

    //link all shaders together
    initShaderProgram();

    //get the variables from the shaders
    initShaderLocations();


    //create the VAO
    glUseProgram(program);
}

void billboard_tech::enable(){
    glUseProgram(program);
}

void billboard_tech::loadShaders()
{
    shaders[0] = loadShader("../shaders/billboardVS.glsl", GL_VERTEX_SHADER);
    shaders[1] = loadShader("../shaders/billboardGS.glsl", GL_GEOMETRY_SHADER);
    shaders[2] = loadShader("../shaders/billboardFS.glsl", GL_FRAGMENT_SHADER);
}

void billboard_tech::initShaderProgram() {
    program = glCreateProgram();
    glAttachShader(program, shaders[0]);
    glAttachShader(program, shaders[1]);
    glAttachShader(program, shaders[2]);



    glLinkProgram(program);

    GLint shader_status;
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if (!shader_status)
    {
        std::cerr << "Unable to create shadevoid billboard_tech::initShaderLocations()";

        char buffer[512];
        glGetProgramInfoLog(program, 512, NULL, buffer); // inserts the error into the buffer
        std::cerr << buffer << std::endl;

        exit(1);
    }
}

void billboard_tech::initShaderLocations()
{
    glUseProgram(program);
    locations["gColorMap"] = glGetUniformLocation(program, "gColorMap");
    locations["gVP"] = glGetUniformLocation(program, "gVP");
    locations["gCameraPos"] = glGetUniformLocation(program, "gCameraPos");
    locations["gBillboardSize"] = glGetUniformLocation(program, "gBillboardSize");

}


void billboard_tech::bind()
{

}

void billboard_tech::unbind()
{

}