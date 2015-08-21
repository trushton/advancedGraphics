#ifndef TECHNIQUE_H
#define TECHNIQUE_H


#include "Program.h"

class Technique :  public Program {
public:

    Technique();

    virtual void init();

    virtual void bind();

    virtual void unbind();

    void enable();

private:

    GLuint m_WVPLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_colorTextureUnitLocation;

protected:
    virtual void loadShaders();

    virtual void initShaderProgram();

    virtual void initShaderLocations();
};


#endif //TECHNIQUE_H