#ifndef DS_SPOT_LIGHT_PASS_TECH_H
#define DS_SPOT_LIGHT_PASS_TECH_H

#include "Program.h"
#include "lights.h"
#include <iostream>

class DSSpotLightPassTech : public Program
    {
    public:
        DSSpotLightPassTech();

        virtual void init();

        virtual void bind();

        virtual void unbind();

        void enable();

        void SetSpotLight(const SpotLight &Light);

    protected:
        virtual void loadShaders();

        virtual void initShaderProgram();

        virtual void initShaderLocations();

    };


#endif