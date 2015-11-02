//
// Created by trushton on 8/11/15.
//
#define GLM_FORCE_RADIANS

#include <glm/gtc/matrix_transform.hpp>
#include "Plane.h"

Plane::~Plane(){

}



void Plane::init(Flag prog, const string textureFile, int pWidth, int pHeight)
{
    program = prog;
    density = .10f;
    width = pWidth;
    height = pHeight;
    Tex = new Texture(textureFile.c_str(), GL_TEXTURE_2D);

    Vertex vert;

    vert.normal[0] = 0;
    vert.normal[1] = 1;
    vert.normal[2] = 0;

    for(float x = 0.0; x < width; x += density)
    {
        for(float z = 0.0; z < height; z += density)
        {
            vert.position[0] = x;
            vert.position[1] = 0;
            vert.position[2] = z;

            vert.texture[0] = x/(float)width;
            vert.texture[1] = z/(float)height;

            geometry.push_back(vert);
        }
    }

    unsigned int scaledWidth = width/density, scaledHeight = height/density;

    for(unsigned int x = 0; x < scaledWidth -1; x++)
    {
        for(unsigned int z = 0; z < scaledHeight -1; z++)
        {
            indices.push_back(z*scaledHeight + x);
            indices.push_back((z*scaledHeight) + x + 1);
            indices.push_back((z+1)* scaledHeight + x);

            indices.push_back((scaledHeight)+x+1);
            indices.push_back((z+1) * scaledHeight + x + 1);
            indices.push_back((z+1) * scaledHeight + x);
        }
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * geometry.size(), geometry.data(), GL_STATIC_DRAW);

    glGenBuffers(1,&vio);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* indices.size(), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(program.getLocation("Position"),
                            3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(program.getLocation("Normal"),
                            3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(program.getLocation("TexCoord"),
                          2, GL_FLOAT, GL_FALSE, sizeof(Vertex),  (void*)offsetof(Vertex,texture));
}


void Plane::tick(float dt)
{

}

void Plane::render()
{
    program.enable();
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    Tex->bind(GL_TEXTURE6);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->m_pImage->columns(), texture->m_pImage->rows(), -0.5, GL_RGBA, GL_UNSIGNED_BYTE,texture->m_blob.data());


    glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Plane::scale(float scaler)
{
    model = glm::scale(model, glm::vec3(scaler, 1, scaler));
}

void Plane::calcNormal(int x, int z, Vertex& vert)
{
    glm::vec3 normal, center, up, down, left, right;

    if(x > 0 && z > 0) {
        center = glm::vec3(vert.position[0], vert.position[1], vert.position[2]);

        up.z = z + 1;
        up.x = x;
        up.y = 0.0f;

        down.z = z - 1;
        down.x = x;
        down.y = 0.0f;

        left.z = z;
        left.x = x - 1;
        left.y = 0.0f;

        right.z = z;
        right.x = x + 1;
        right.y = 0.0f;

        normal += glm::cross(up-center, right-center);
        normal += glm::cross(right-center, down-center);
        normal += glm::cross(down-center, left-center);
        normal += glm::cross(left-center, up-center);
        normal  = glm::normalize(normal);

        vert.normal[0] = normal.x;
        vert.normal[1] = normal.y;
        vert.normal[2] = normal.z;
    }
}