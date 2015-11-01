//
// Created by trushton on 7/27/15.
//

#include "grass_tech.h"

grass_tech::grass_tech(){}

void grass_tech::init(){
    //load and compile shaders
    loadShaders();

    //link all shaders together
    initShaderProgram();

    //get the variables from the shaders
    initShaderLocations();

    tex = new Texture("../bin/terrain/grass.png", GL_TEXTURE_2D);
    pathTex = new Texture("../bin/terrain/path3.jpg", GL_TEXTURE_2D);

    //create the VAO
    glUseProgram(program);
}

void grass_tech::enable(){
    glUseProgram(program);
}

void grass_tech::loadShaders()
{
    shaders[0] = loadShader("../shaders/grassVS.glsl", GL_VERTEX_SHADER);
    shaders[1] = loadShader("../shaders/grassGS.glsl", GL_GEOMETRY_SHADER);
    shaders[2] = loadShader("../shaders/grassFS.glsl", GL_FRAGMENT_SHADER);
}

void grass_tech::initShaderProgram() {
    program = glCreateProgram();
    glAttachShader(program, shaders[0]);
    glAttachShader(program, shaders[1]);
    glAttachShader(program, shaders[2]);

    glLinkProgram(program);

    GLint shader_status;
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if (!shader_status)
    {
        std::cerr << "Unable to create shadevoid grass_tech::initShaderLocations()";

        char buffer[512];
        glGetProgramInfoLog(program, 512, NULL, buffer); // inserts the error into the buffer
        std::cerr << buffer << std::endl;

        exit(1);
    }
}

void grass_tech::initShaderLocations()
{
    glUseProgram(program);
    locations["Position"] = glGetAttribLocation(program, "Position");
    locations["model"] = glGetUniformLocation(program, "model");
    locations["gVP"] = glGetUniformLocation(program, "gVP");
    locations["gCameraPos"] = glGetUniformLocation(program, "gCameraPos");
    locations["time"] = glGetUniformLocation(program, "time");
    locations["gColorMap"] = glGetUniformLocation(program, "gColorMap");
    locations["gPathMap"] = glGetUniformLocation(program, "gPathMap");
    locations["renderScale"] = glGetUniformLocation(program, "renderScale");
    locations["fMaxTextureU"] = glGetUniformLocation(program, "fMaxTextureU");
    locations["fMaxTextureV"] = glGetUniformLocation(program, "fMaxTextureV");



}


void grass_tech::bind()
{

}

void grass_tech::unbind()
{

}

