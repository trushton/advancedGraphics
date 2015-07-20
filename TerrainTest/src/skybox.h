//
// Created by eric on 6/25/15.
//

#ifndef Skybox_H
#define Skybox_H

#include "Shader.h"
//#include "model.h"
#include "Program.h"
#include <vector>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"

#define INDEX_BUFFER 0
#define POS_VB 1


class Skybox : public Program  {

public:

    Skybox();

    virtual void init();
    virtual void bind();
    virtual void unbind();


    void tick(float dt);
    void render();

private:
    void loadShaders();
    void initShaderProgram();
    void initShaderLocations();


    // The GLSL program handle
    GLuint m_VAO;


    // transform matrices
    glm::mat4 view;
    glm::mat4 projection;

    string m_fileNames[6];
    GLuint skyboxTexture;

};


#endif //Skybox_H
