#ifndef PS_UPDATE_TECHNIQUE_H
#define	PS_UPDATE_TECHNIQUE_H

#include "Program.h"
#include <iostream>


class PSUpdate : public Program
{
    public:
        glm::mat4 model;

        PSUpdate();

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
#endif	/* PS_UPDATE_TECHNIQUE_H */

