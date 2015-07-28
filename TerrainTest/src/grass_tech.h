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

        virtual void init();

        virtual void bind();

        virtual void unbind();

        void enable();

        //void SetPointLight(const PointLight& Lights);

    protected:
        virtual void loadShaders();

        virtual void initShaderProgram();

        virtual void initShaderLocations();
};


#endif
