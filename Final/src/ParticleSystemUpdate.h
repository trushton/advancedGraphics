//
// Created by trushton on 8/22/15.
//

#ifndef FINAL_PARTICLESYSTEMUPDATE_H
#define FINAL_PARTICLESYSTEMUPDATE_H


#include "Program.h"
#include <iostream>

class ParticleUpdateProgram : public Program
{
public:
    ParticleUpdateProgram();

    void init();
    void enable();
    void bind();

    void unbind();

private:

    void loadShaders();
    void initShaderProgram();
    void initShaderLocations();
};

#endif
