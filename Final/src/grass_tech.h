#ifndef TERRAINTEST_GRASS_TECH_H
#define TERRAINTEST_GRASS_TECH_H
#include "Program.h"
#include "lights.h"
#include <iostream>
#include <chrono>

class grass_tech : public Program
{
    public:
        glm::mat4 model;
        grass_tech();

        void init();

        void bind();

        void unbind();

        void enable();

        //void SetPointLight(const PointLight& Lights);

    protected:
        void loadShaders();

        void initShaderProgram();

        void initShaderLocations();
};


#endif
