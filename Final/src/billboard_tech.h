#ifndef BILLBOARD_TECH_H
#define	BILLBOARD_TECH_H
#include "Program.h"
#include "lights.h"
#include <iostream>
#include <chrono>

class billboard_tech : public Program
{
    public:
        glm::mat4 model;
        billboard_tech();

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
