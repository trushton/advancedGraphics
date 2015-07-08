#ifndef DS_GEOM_PASS_TECH_H
#define	DS_GEOM_PASS_TECH_H

#include <glm/detail/type_mat.hpp>
#include <GL/glew.h>
#include "technique.h"

class DSGeomPassTech : public Technique {
public:

    DSGeomPassTech();

    virtual bool Init();

    void SetMVP(const glm::mat4 mvp);
    void SetModelMatrix(const glm::mat4 model);
    void SetTextureUnit(unsigned int TextureUnit);


    GLuint loc_mvpMatrix;
    GLuint loc_modelMatrix;
    GLuint loc_texture;
};


#endif