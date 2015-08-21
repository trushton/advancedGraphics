#ifndef DS_POINT_LIGHT_PASS_TECH_H
#define	DS_POINT_LIGHT_PASS_TECH_H

#include "Program.h"
#include "lights.h"
#include <iostream>

class DSPointLightPassTech : public Program
{
public:
    DSPointLightPassTech();

    virtual void init();

    virtual void bind();

    virtual void unbind();

    void enable();

    void SetPointLight(const PointLight& Lights);

protected:
    virtual void loadShaders();

    virtual void initShaderProgram();

    virtual void initShaderLocations();

};


#endif
