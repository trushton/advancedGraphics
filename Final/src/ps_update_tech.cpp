//
// Created by trushton on 7/28/15.
//

#include "ps_update_tech.h"

PSUpdate::PSUpdate() { }

void PSUpdate::init(){
    //load and compile shaders
    loadShaders();

    //link all shaders together
    initShaderProgram();

    //get the variables from the shaders
    initShaderLocations();


    //create the VAO
    glUseProgram(program);
}

void PSUpdate::enable(){
    glUseProgram(program);
}

void PSUpdate::loadShaders()
{
    shaders[0] = loadShader("../shaders/particleVS.glsl", GL_VERTEX_SHADER);
    shaders[1] = loadShader("../shaders/particleGS.glsl", GL_GEOMETRY_SHADER);
    //shaders[2] = loadShader("../shaders/particleFS.glsl", GL_FRAGMENT_SHADER);
}

void PSUpdate::initShaderProgram() {
    program = glCreateProgram();
    glAttachShader(program, shaders[0]);
    glAttachShader(program, shaders[1]);
    //glAttachShader(program, shaders[2]);

    const GLchar* Varyings[4];
    Varyings[0] = "Type1";
    Varyings[1] = "Position1";
    Varyings[2] = "Velocity1";
    Varyings[3] = "Age1";

    glTransformFeedbackVaryings(program, 4, Varyings, GL_INTERLEAVED_ATTRIBS);

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

void PSUpdate::initShaderLocations()
{
    glUseProgram(program);
    locations["gDeltaTime"] = glGetAttribLocation(program, "gDeltaTime");
    locations["gTime"] = glGetUniformLocation(program, "gTime");
    locations["gRandomTexture"] = glGetUniformLocation(program, "gRandomTexture");
    locations["gLauncherLife"] = glGetUniformLocation(program, "gLauncherLife");
    locations["gShellLife"] = glGetUniformLocation(program, "gShellLife");
    locations["gSecondaryLife"] = glGetUniformLocation(program, "gSecondaryLife");
}


void PSUpdate::bind()
{

}

void PSUpdate::unbind() {

}