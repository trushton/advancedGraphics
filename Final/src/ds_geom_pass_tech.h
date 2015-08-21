#ifndef DS_GEOM_PASS_TECH_H
#define	DS_GEOM_PASS_TECH_H

#include "Program.h"

class DSGeomPassTech :  public Program {
public:

    DSGeomPassTech();

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


#endif
