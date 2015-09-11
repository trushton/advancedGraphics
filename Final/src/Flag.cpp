//
// Created by trushton on 8/11/15.
//

#include "Flag.h"
#include "Engine.h"

Flag::Flag(){
    init();
}

Flag::~Flag()
{

}

void Flag::init()
{
    //load and compile shaders
    loadShaders();

    //link all shaders together
    initShaderProgram();

    //get the variables from the shaders
    initShaderLocations();


    //create the VAO
    glUseProgram(program);

}

void Flag::enable()
{
    //use this program
    glUseProgram(program);
}

void Flag::loadShaders()
{
    shaders[0] = loadShader("../shaders/flag.vs", GL_VERTEX_SHADER);
    shaders[1] = loadShader("../shaders/geometry_pass.fs", GL_FRAGMENT_SHADER);
}

void Flag::initShaderProgram()
{
    program = glCreateProgram();
    glAttachShader(program, shaders[0]);
    glAttachShader(program, shaders[1]);

    glLinkProgram(program);

    GLint shader_status;
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "Unable to create shadevoid grass_tech::initShaderLocations()";

        char buffer[512];
        glGetProgramInfoLog(program, 512, NULL, buffer); // inserts the error into the buffer
        std::cerr << buffer << std::endl;

        exit(1);
    }
}

void Flag::initShaderLocations()
{
    glUseProgram(program);
    locations["Position"] = glGetAttribLocation(program, "Position");
    locations["Normal"] = glGetAttribLocation(program, "Normal");
    locations["TexCoord"] = glGetAttribLocation(program, "TexCoord");

    locations["gWVP"] = glGetUniformLocation(program, "gWVP");
    locations["gWorld"] = glGetUniformLocation(program, "gWorld");
    locations["gColorMap"] = glGetUniformLocation(program, "gColorMap");

    locations["waveTime"] = glGetUniformLocation(program, "waveTime");
    locations["waveWidth"] = glGetUniformLocation(program, "waveWidth");
    locations["waveHeight"] = glGetUniformLocation(program, "waveHeight");


}

void Flag::bind()
{

}

void Flag::unbind()
{

}