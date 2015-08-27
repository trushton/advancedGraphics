//
// Created by trushton on 8/27/15.
//

#ifndef FINAL_WATERPLANE_H
#define FINAL_WATERPLANE_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "Engine.h"
#include "Program.h"
#include "Texture.h"
#include "Flag.h"
#include "water_tech.h"
#include <GL/glew.h>
#include <iostream>

using namespace std;

class waterPlane {
public:
    virtual ~waterPlane();

    virtual void init(water_tech prog, const string textureFile, int pWidth, int pHeight);

    virtual void tick(float dt);
    virtual void render();

    virtual void scale(float scaler);

    glm::mat4 model;

private:
    void calcNormal(int x, int z, Vertex& vert);

    vector<unsigned int> indices;
    water_tech program;
    Texture *Tex;
    int width, height;
    vector<Vertex> geometry;
    string textureFile;
    GLuint vio;
    GLuint vbo, vao;
    float density;
};


#endif //FINAL_WATERPLANE_H
