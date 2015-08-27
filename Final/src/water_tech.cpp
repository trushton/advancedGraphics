//
// Created by trushton on 8/3/15.
//

#include "water_tech.h"
#include "Engine.h"
#include "graphics.h"

water_tech::water_tech(){}

void water_tech::init(){
    //load and compile shaders
    loadShaders();

    //link all shaders together
    initShaderProgram();

    //get the variables from the shaders
    initShaderLocations();


    //create the VAO
    glUseProgram(program);


}

void water_tech::enable(){
    glUseProgram(program);
}

void water_tech::loadShaders()
{
    shaders[0] = loadShader("../shaders/waterVS.vs", GL_VERTEX_SHADER);
    shaders[1] = loadShader("../shaders/flag.fs", GL_FRAGMENT_SHADER);
}

void water_tech::initShaderProgram() {
    program = glCreateProgram();
    glAttachShader(program, shaders[0]);
    glAttachShader(program, shaders[1]);
    //glAttachShader(program, shaders[2]);

    glLinkProgram(program);

    GLint shader_status;
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if (!shader_status)
    {
        std::cerr << "Unable to create shadevoid water_tech::initShaderLocations()";

        char buffer[512];
        glGetProgramInfoLog(program, 512, NULL, buffer); // inserts the error into the buffer
        std::cerr << buffer << std::endl;

        exit(1);
    }
}

void water_tech::initShaderLocations()
{
    glUseProgram(program);
    locations["Position"] = glGetAttribLocation(program, "Position");
    locations["Normal"] = glGetAttribLocation(program, "Normal");
    locations["TexCoord"] = glGetAttribLocation(program, "TexCoord");
    locations["time"] = glGetAttribLocation(program, "time");


    locations["gWVP"] = glGetUniformLocation(program, "gWVP");
    locations["gWorld"] = glGetUniformLocation(program, "gWorld");
    locations["gColorMap"] = glGetUniformLocation(program, "gColorMap");

    locations["waveTime"] = glGetUniformLocation(program, "waveTime");
    locations["waveWidth"] = glGetUniformLocation(program, "waveWidth");
    locations["waveHeight"] = glGetUniformLocation(program, "waveHeight");
}


void water_tech::bind()
{

}

void water_tech::unbind()
{

}
