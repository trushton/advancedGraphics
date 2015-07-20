
#ifndef DS_DIR_LIGHT_PASS_TECH_H
#define	DS_DIR_LIGHT_PASS_TECH_H

#include "Program.h"
#include "lights.h"

#include <iostream>

class DSDirLightPassTech : public Program {
public:
    DSDirLightPassTech();

    virtual void init();

    virtual void bind();

    virtual void unbind();

    void enable();

    void SetDirectionalLight(const DirectionalLight& Light);

protected:
    virtual void loadShaders();

    virtual void initShaderProgram();

    virtual void initShaderLocations();

};


#endif