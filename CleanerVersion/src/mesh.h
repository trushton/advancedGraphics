// This code is based on the example code provided by this link: http://ogldev.atspace.co.uk/www/tutorial22/tutorial22.html

#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include "Vertex.h"
#include <FreeImagePlus.h>

class Mesh {

public:
    Mesh();
    void setData();

    std::vector<textureVertex> vertices;
    std::vector<unsigned int> indices;

    GLuint VBO;
    GLuint IB;
    GLuint TB;

    unsigned int numIndices;
    unsigned int baseVertex;
    unsigned int baseIndex;
    unsigned int materialIndex;

    fipImage image;
};
#endif