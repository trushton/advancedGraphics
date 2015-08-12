//
// Created by trushton on 8/11/15.
//

#ifndef FLAG_H
#define FLAG_H
#include "Program.h"
#include <iostream>



class Flag : public Program{

public:
    Flag();
    virtual ~Flag();

    void init();
    void enable();
    void bind();
    void unbind();

private:
    GLuint m_WVPLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_colorTextureUnitLocation;

protected:
    void loadShaders();
    void initShaderProgram();
    void initShaderLocations();
};


#endif
