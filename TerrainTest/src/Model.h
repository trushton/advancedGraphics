// This code is based on the example code provided by this link: http://ogldev.atspace.co.uk/www/tutorial22/tutorial22.html


#ifndef MODEL_H
#define    MODEL_H

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"

#include <assert.h>
//#include "lightingEngine.h"

#define INDEX_BUFFER 0
#define POS_VB 1
#define NORMAL_VB 2
#define TEXCOORD_VB 3


using std::vector; using std::cout; using std::endl; using std::string;

class Model
{

public:
    glm::mat4 model; // obj->world each object should have its own model matrix
    glm::mat4 mvp; // premultiplied modelviewprojection

    bool loadModel(const std::string &Filename);

    void renderModel();
    void renderModel(GLuint texture);


    void deleteModel();

private:

    bool initializeModel(const aiScene *scene, const std::string &filename);

    void initializeMesh(unsigned int index, const aiScene *scene, aiColor3D color,
                        std::vector<glm::vec3> &positions, std::vector<glm::vec3>& normals, std::vector<unsigned int> &indices,
                        std::vector<glm::vec2> &texture);

    void initializeMaterials(const aiScene *pScene, unsigned index);

    vector<Mesh> meshList;

    GLuint m_VAO;
    GLuint m_Buffers[4];

};

#endif