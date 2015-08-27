//
// Created by trushton on 8/3/15.
//

#ifndef TERRAINTEST_WATER_TECH_H
#define TERRAINTEST_WATER_TECH_H
#include "Program.h"
#include "lights.h"
#include "Model.h"
#include <iostream>
#include <chrono>

class water_tech : public Program
{
public:
    water_tech();

    void init();

    void bind();

    void unbind();

    void enable();



protected:
    void loadShaders();

    void initShaderProgram();

    void initShaderLocations();

    std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;
    float time;
};


#endif //TERRAINTEST_WATER_TECH_H
