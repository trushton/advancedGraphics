#include "Mesh.h"
#include <assert.h>


void Mesh::setData()
{
    // store the number of indices
    numIndices = indices.size();

    // bind and setup the vertex buffer
    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // bind and setup the indices buffer
    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &indices[0], GL_STATIC_DRAW);
}