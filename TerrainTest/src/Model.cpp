// This code is based on the example code provided by this link: http://ogldev.atspace.co.uk/www/tutorial22/tutorial22.html


/******************** Header Files ********************/
#include "Model.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))


/************ Constructors and Destructors ************/
bool Model::loadModel(const string &filename)
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    // initialize varaibles
    bool status = false;
    Assimp::Importer importer;

    // read in the model file and store it as a scene
    const aiScene *scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate |aiProcess_GenSmoothNormals );

    // check if the scene has been loaded correctly
    if (scene)
    {
        // initialize the imported model
        status = initializeModel(scene, filename);
    }
        // throw and error for the model not loading correctly
    else
    {
        // display error for failed loading
        cout << "Error parsing " << filename << ": " << importer.GetErrorString() << endl;
    }

    glBindVertexArray(0);

    return status;
}


bool Model::initializeModel(const aiScene *scene, const string &filename)
{
    // initialize variables
    meshList.resize(scene->mNumMeshes);
    aiColor3D color(0.0f, 0.0f, 0.0f);
    aiMesh *mesh;
    aiMaterial *pMaterial;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    std::vector<glm::vec2> textureCoords;

    unsigned int NumVertices = 0, NumIndices = 0;

    // Count the number of vertices and indices
    for (unsigned int i = 0 ; i < meshList.size() ; i++) {
        meshList[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;
        meshList[i].numIndices = scene->mMeshes[i]->mNumFaces * 3;
        meshList[i].baseVertex = NumVertices;
        meshList[i].baseIndex = NumIndices;

        NumVertices += scene->mMeshes[i]->mNumVertices;
        NumIndices += meshList[i].numIndices;
    }


    // loop and initialize all the meshes
    for (unsigned int i = 0; i < meshList.size(); i++)
    {
        // get the mesh from the scene
        mesh = scene->mMeshes[i];

        // get the material for the mesh
        pMaterial = scene->mMaterials[mesh->mMaterialIndex];

        // get the diffuse color from the material
        pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);

        // initialize the mesh
        initializeMesh(i, scene, color,positions, normals, indices, textureCoords);
    }

    // Generate and populate the buffers with vertex attributes and the indices
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords[0]) * textureCoords.size(), &textureCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // return
    return true;

}


void  Model::initializeMesh(unsigned int index, const aiScene *scene, aiColor3D color, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices, std::vector<glm::vec2>& textureCoord)
{
    // initialize variables
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    aiVector3D *position;
    aiVector3D *position2;
    textureVertex v;

    // get the correct mesh
    aiMesh *mesh = scene->mMeshes[index];

    // set the location of the material
    meshList[index].materialIndex = mesh->mMaterialIndex;

    // get all the mesh vertices and store them in the vertices vector
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // get the vertice coordinate
        position = &(mesh->mVertices[i]);

        // store vertice position in vertex struct
        positions.push_back(glm::vec3(position->x, position->y, position->z));

        // Then add the triangle to the mesh:

        // store vertice color in vertex struct
        if (mesh->HasTextureCoords(0))
        {
            aiVector3D t = mesh->mTextureCoords[0][i];
            textureCoord.push_back(glm::vec2(t.x, t.y));
        }
        else
        {
            textureCoord.push_back(glm::vec2(0, 0));
        }

        if (mesh->HasNormals()) {
            const aiVector3D* normal = &(mesh->mNormals[i]);
            normals.push_back(glm::vec3(normal->x, normal->y, normal->z));
        }

        // store the vertex struct in a vector
        // meshList[index].vertices.push_back(v);
    }

    // get all the mesh faces and store them in the indices vector
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {

        // get the face from the mesh
        const aiFace &face = mesh->mFaces[i];

        // check to make sure there are 3 indices
        //cout << face.mNumIndices << endl;
        assert(face.mNumIndices == 3);

        // store the indices in the indices vector
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }


    // get texture
    initializeMaterials(scene, index);

    // store the mesh data in GPU memory
    // meshList[index].setData();

}


void Model::initializeMaterials(const aiScene *scene, unsigned meshIndex)
{
    // initialize variables
    aiString filepath;

    // get the material info for the mesh
    const aiMaterial *material = scene->mMaterials[meshList[meshIndex].materialIndex];

    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &filepath, NULL, NULL, NULL, NULL, NULL) ==
            AI_SUCCESS)
        {
            // load texture image
            aiString path("../bin/");
            path.Append(filepath.C_Str());
            if (meshList[meshIndex].image.load(path.C_Str()))
            {
                meshList[meshIndex].image.convertTo32Bits();

                // bind the texture data buffer
                glGenTextures(1, &meshList[meshIndex].TB);
                glBindTexture(GL_TEXTURE_2D, meshList[meshIndex].TB);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, meshList[meshIndex].image.getWidth(),
                             meshList[meshIndex].image.getHeight(),
                             0, GL_BGRA, GL_UNSIGNED_BYTE,
                             (GLvoid *) meshList[meshIndex].image.accessPixels());
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            }
            else
            {
                cout << "Texture: " << path.C_Str() << " failed to load" << endl;
            }
        }
    }
}

void Model::renderModel()
{
    glBindVertexArray(m_VAO);

    for (unsigned int i = 0 ; i < meshList.size() ; i++) {
        const unsigned int MaterialIndex = meshList[i].materialIndex;


        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, meshList[i].TB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, meshList[i].image.getWidth(), meshList[i].image.getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid *) meshList[i].image.accessPixels());


        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 meshList[i].numIndices,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(unsigned int) * meshList[i].baseIndex),
                                 meshList[i].baseVertex);
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);
}

void Model::renderModel(GLuint texture)
{
    glBindVertexArray(m_VAO);

    for (unsigned int i = 0 ; i < meshList.size() ; i++) {
        const unsigned int MaterialIndex = meshList[i].materialIndex;


        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, texture);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, meshList[i].image.getWidth(), meshList[i].image.getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid *) meshList[i].image.accessPixels());


        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 meshList[i].numIndices,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(unsigned int) * meshList[i].baseIndex),
                                 meshList[i].baseVertex);
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);
}


void Model::deleteModel()
{
    for (int i = 0; i < meshList.size(); i++)
    {
        // delete the vertices GPU memory
        //   glDeleteBuffers(1, &meshList[i].VBO);

        // delete the indices GPU memory
        //  glDeleteBuffers(1, &meshList[i].IB);

        // glDeleteVertexArrays(1, &meshList[i].VAO);
    }
    meshList.clear();

}
