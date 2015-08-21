// This code is based on the example code provided by this link: http://ogldev.atspace.co.uk/www/tutorial22/tutorial22.html

/******************** Header Files ********************/
#include "mesh.h"
#include <assert.h>

Mesh::Mesh()
{
    numIndices = 0;
    baseVertex = 0;
    baseIndex = 0;

}



void Mesh::setData()
{

    // store the number of indices
    numIndices = indices.size();

    // bind and setup the vertex buffer

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // bind and setup the indices buffer
    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &indices[0], GL_STATIC_DRAW);

    // This ensures the VAO is not changed from outside code

}