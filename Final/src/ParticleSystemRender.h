//
// Created by trushton on 8/22/15.
//

#ifndef FINAL_PARTICLESYSTEMRENDER_H
#define FINAL_PARTICLESYSTEMRENDER_H

#include "Program.h"
#include <iostream>

class ParticleRenderProgram : public Program
{
public:
    ParticleRenderProgram();

    void init();
    void enable();
    void bind();

    void unbind();

private:
    void loadShaders();
    void initShaderProgram();
    void initShaderLocations();
};


#endif //FINAL_PARTICLESYSTEMRENDER_H
