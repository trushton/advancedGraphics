//
// Created by trushton on 9/11/15.
//

#ifndef FINAL_TERRAIN_H
#define FINAL_TERRAIN_H
#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include "Texture.h"
#include "Vertex.h"
#include <fstream>

#include "Program.h"
#include "grass_tech.h"

class Terrain : public Program {
public:
    glm::mat4 model;
    Terrain(glm::vec3 scale, std::string filename);
    Terrain();

    void init();
    bool initialize();
    bool buildTerrain();
    void render(float dt);

    void bind();

    void unbind();

    void enable();

    //void SetPointLight(const PointLight& Lights);

    glm::vec3 location, renderScale;
    std::string fname;
    float time;
    Texture* grassPath;
    std::vector<GLM_Vertex> Vertices;

    void CreatePositionBuffer();
    void RenderGrass();
    void ReadPath();
protected:
    void loadShaders();

    void initShaderProgram();

    void initShaderLocations();

    std::vector<Texture*> texture;
    GLint TextureLocations[5];
    GLint Color;

    Magick::Image m_image;
    Magick::Blob m_blob;
    int irows, icols, isize;

    GLuint VB;
    GLuint IB;
    std::vector<unsigned int> Indices;
    std::vector<glm::vec3> pathPos;


    GLint RenderHeight;
    GLint MaxTextureU;
    GLint MaxTextureV;
    GLint HeightmapScale;
    GLint ProjMatrix;
    GLint ViewMatrix;
    GLint ModelMatrix;
    GLint NormalMatrix;

    std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;

    GLuint m_Buffers[3];
    grass_tech grass;

    std::vector<glm::vec3> positions;
    GLuint grass_VB;
};


#endif //FINAL_TERRAIN_H
