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

    plane.loadModel("../bin/plane.obj");
    plane.model = glm::mat4(1.0f);
}

void water_tech::enable(){
    glUseProgram(program);
}

void water_tech::loadShaders()
{
    shaders[0] = loadShader("../shaders/waterVS.glsl", GL_VERTEX_SHADER);
    //shaders[1] = loadShader("../shaders/waterGS.glsl", GL_GEOMETRY_SHADER);
    shaders[1] = loadShader("../shaders/waterFS.glsl", GL_FRAGMENT_SHADER);
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
    locations["mvp"] = glGetUniformLocation(program, "mvp");
    locations["time"] = glGetUniformLocation(program, "time");
}


void water_tech::bind()
{

}

void water_tech::unbind()
{

}

void water_tech::RenderWater(){
    t2 = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration_cast<std::chrono::duration<float> >(t2 - t1).count();
    set("mvp", Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view * model);
    set("time", time);

    plane.renderModel();
}