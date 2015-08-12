//
// Created by trushton on 8/11/15.
//

#ifndef PLANE_H
#define PLANE_H
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "Engine.h"
#include "Program.h"
#include "Texture.h"
#include "Flag.h"
#include <GL/glew.h>
#include <iostream>

using namespace std;

class Plane {
    public:
        virtual ~Plane();

        virtual void init(Flag prog, const string textureFile, int pWidth, int pHeight);
        virtual void tick(float dt);
        virtual void render();
        virtual void scale(float scaler);

        glm::mat4 model;

private:
        void calcNormal(int x, int z, Vertex& vert);

        vector<unsigned int> indices;
        Flag program;
        Texture *texture;
        int width, height;
        vector<Vertex> geometry;
        string textureFile;
        GLuint vio;
        GLuint vbo, vao, m_Buffers[4];
        float density;
};


#endif
