#ifndef VERTEX_H
#define VERTEX_H

#include <GL/glew.h>


struct Vertex
    {
        GLfloat position[3];
        GLfloat normal[3];
        GLfloat texture[3];
    };

struct textureVertex
{
    GLfloat position[3];
    GLfloat uv[2];
};

#endif // VERTEX_H