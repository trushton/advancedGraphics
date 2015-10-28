//
// Created by trushton on 9/11/15.
//
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Terrain.h"
#include "Engine.h"
#include "graphics.h"

Terrain::Terrain(glm::vec3 scale, std::string filename){
    renderScale = scale;
    fname = filename;
}

Terrain::Terrain(){};

void Terrain::init(){

}

bool Terrain::initialize(){
    t1 = std::chrono::high_resolution_clock::now();

    //load and compile shaders
    loadShaders();

    //link all shaders together
    initShaderProgram();

    //get the variables from the shaders
    initShaderLocations();

    texture.resize(6);
    texture[0] = new Texture("../bin/terrain/sand.jpg", GL_TEXTURE_2D);
    texture[1] = new Texture("../bin/terrain/grassTex.jpg", GL_TEXTURE_2D);
    texture[2] = new Texture("../bin/terrain/forest.jpg", GL_TEXTURE_2D);
    texture[3] = new Texture("../bin/terrain/rock_2_4w.jpg", GL_TEXTURE_2D);
    texture[4] = new Texture("../bin/terrain/stone.jpg", GL_TEXTURE_2D);
    texture[5] = new Texture("../bin/terrain/path3.jpg", GL_TEXTURE_2D);
    grassPath = new Texture("../bin/terrain/path3.jpg", GL_TEXTURE_2D);


    //create the VAO
    glUseProgram(program);

    //build the terrain
    return buildTerrain();
}

void Terrain::enable(){
    glUseProgram(program);
}

void Terrain::loadShaders() {
    shaders[0] = loadShader("../shaders/terrain_vertex.glsl", GL_VERTEX_SHADER);
    shaders[1] = loadShader("../shaders/terrain_fragment.glsl", GL_FRAGMENT_SHADER);
}

void Terrain::initShaderProgram() {
    program = glCreateProgram();
    glAttachShader(program, shaders[0]);
    glAttachShader(program, shaders[1]);

    glLinkProgram(program);

    GLint shader_status;
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if (!shader_status)
    {
        std::cerr << "Unable to create shadevoid grass_tech::initShaderLocations()";

        char buffer[512];
        glGetProgramInfoLog(program, 512, NULL, buffer); // inserts the error into the buffer
        std::cerr << buffer << std::endl;

        exit(1);
    }
}

void Terrain::initShaderLocations()
{
    glUseProgram(program);

    locations["time"] = glGetUniformLocation(program, "time");

    locations["TextureLocations[0]"] = glGetUniformLocation(program, "gSampler[0]");
    locations["TextureLocations[1]"] = glGetUniformLocation(program, "gSampler[1]");
    locations["TextureLocations[2]"] = glGetUniformLocation(program, "gSampler[2]");
    locations["TextureLocations[3]"] = glGetUniformLocation(program, "gSampler[3]");
    locations["TextureLocations[4]"] = glGetUniformLocation(program, "gSampler[4]");
    locations["TextureLocations[5]"] = glGetUniformLocation(program, "gSampler[5]");


    locations["RenderHeight"] = glGetUniformLocation(program, "fRenderHeight");
    locations["MaxTextureU"] = glGetUniformLocation(program, "fMaxTextureU");
    locations["MaxTextureV"] = glGetUniformLocation(program, "fMaxTextureV");
    locations["HeightmapScale"] = glGetUniformLocation(program, "HeightmapScaleMatrix");
    locations["ProjMatrix"] = glGetUniformLocation(program, "matrices.projMatrix");
    locations["ViewMatrix"] = glGetUniformLocation(program, "matrices.viewMatrix");
    locations["ModelMatrix"] = glGetUniformLocation(program, "matrices.modelMatrix");
    locations["NormalMatrix"] = glGetUniformLocation(program, "matrices.normalMatrix");

    locations["Color"] = glGetUniformLocation(program, "vColor");


}

bool Terrain::buildTerrain() {
    try{
        m_image.read(fname);
        m_image.write(&m_blob, "RGBA");
    }
    catch(Magick::Error& Error){
        std::cout << "Error loading texture in terrain '" << fname << "': " << Error.what() << std::endl;
        return false;
    }

    irows = m_image.rows();
    icols = m_image.columns();

    uint ptr_inc = m_image.depth() == 24 ? 3 : 1;
    uint rowStep = ptr_inc * icols;

    // Build Vertex and textures
    std::vector<std::vector<glm::vec3>> vVertexData(irows, std::vector<glm::vec3>(icols));
    std::vector<std::vector<glm::vec2>> vCoordsData(irows, std::vector<glm::vec2>(icols));

    float fTextureU = float(icols)*0.1f;
    float fTextureV = float(irows)*0.1f;

    const void* data = m_blob.data();

    for(int i = 0; i < irows; i++)
    {
        for(int j = 0; j < icols; j++)
        {
            float fScaleC = float(j)/float(icols-1);
            float fScaleR = float(i)/float(irows-1);

            float fVertexHeight = float(*(static_cast<int const *>(data) + rowStep * i + j * ptr_inc))/65535.0f;
            //printf("The Height: %.5f\n", -fVertexHeight/255.0f);

            vVertexData[i][j] = glm::vec3(-0.5f+fScaleC, 1-(-fVertexHeight/255.0f), -0.5f+fScaleR);
            vCoordsData[i][j] = glm::vec2(fTextureU*fScaleC, fTextureV*fScaleR);
        }
    }

    // Build normals
    std::vector< std::vector<glm::vec3> > vNormals[2];
    for(int i = 0; i < 2; i++)
    {
        vNormals[i] = std::vector< std::vector<glm::vec3> >(irows-1, std::vector<glm::vec3>(icols-1));
    }

    for(int i = 0; i < irows-1; i++)
    {
        for(int j = 0; j < icols-1; j++)
        {
            glm::vec3 vTriangle0[] = { vVertexData[i][j], vVertexData[i+1][j], vVertexData[i+1][j+1]};
            glm::vec3 vTriangle1[] = { vVertexData[i+1][j+1], vVertexData[i][j+1], vVertexData[i][j]};

            glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0]-vTriangle0[1], vTriangle0[1]-vTriangle0[2]);
            glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0]-vTriangle1[1], vTriangle1[1]-vTriangle1[2]);

            vNormals[0][i][j] = glm::normalize(vTriangleNorm0);
            vNormals[1][i][j] = glm::normalize(vTriangleNorm1);
        }
    }

    // Sum Normals, and normalize
    std::vector< std::vector<glm::vec3> > vFinalNormals = std::vector< std::vector<glm::vec3> >(irows, std::vector<glm::vec3>(icols));

    for(int i = 0; i < irows; i++)
    {
        for(int j = 0; j < icols; j++)
        {
            glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);

            // Look for upper-left triangles
            if(j != 0 && i != 0)
            {
                for(int k = 0; k < 2; k++)
                {
                    vFinalNormal += vNormals[k][i-1][j-1];
                }
            }

            // Look for upper-right triangles
            if(i != 0 && j != icols-1)
            {
                vFinalNormal += vNormals[0][i-1][j];
            }

            // Look for bottom-right triangles
            if(i != irows-1 && j != icols-1)
            {
                for(int k = 0; k < 2; k++)
                {
                    vFinalNormal += vNormals[k][i][j];
                }
            }

            // Look for bottom-left triangles
            if(i != irows-1 && j != 0)
            {
                vFinalNormal += vNormals[1][i][j-1];
            }

            vFinalNormal = glm::normalize(vFinalNormal);
            vFinalNormals[i][j] = vFinalNormal;
        }
    }

    // Build the buffer

    for(int i = 0; i < irows; i++)
    {
        for(int j = 0; j < icols; j++)
        {


            Vertices.push_back(GLM_Vertex(glm::vec3(vVertexData[i][j].x , vVertexData[i][j].y , vVertexData[i][j].z ),
                                          vCoordsData[i][j],
                                          vFinalNormals[i][j],
                                          glm::vec3(0, 0, 0)));
        }
    }

    //int iPrimitiveRestartIndex = irows * icols;

    printf("irows: %i, icols: %i\n", irows, icols);
    for(int i = 0; i < irows-1; i++)
    {
        for(int j = 0; j < icols-1; j++)
        {
            /* 
             //printf("Indicies: ");
             for(int k = 0; k < 2; k++)
             { 
               int iRow = i+(1-k); 
               int iIndex = iRow*icols+j; 
               Indices.push_back((unsigned int)iIndex);
               //printf("%d, ", iIndex);
             }
             */
            if(j == 0 || j == 1 || j == icols-1 || j == icols-2)
            {
                //printf("%i, %i, %i, %i, %i, %i\n", (i * irows)+j+1, (i * irows)+j, ((1+i) * irows)+j+1, ((1+i) * irows)+j+2, (i * irows)+j+1, ((1+i) * irows)+j+1);
            }
            Indices.push_back((i * irows)+j+1);
            Indices.push_back((i * irows)+j);
            Indices.push_back(((1+i) * irows)+j);

            Indices.push_back(((1+i) * irows)+j);
            Indices.push_back(((1+i) * irows)+j+1);
            Indices.push_back((i * irows)+j+1);

            // Restart triangle strips
            //printf("%d\n", iPrimitiveRestartIndex);
            //Indices.push_back((unsigned int)iPrimitiveRestartIndex);

        }
    }

    isize = Indices.size(); //((irows-1)*icols*2 + irows-1);

    // Add vertex and indices's buffer
    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLM_Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    grass.init();
    ReadPath();
    CreatePositionBuffer();
    return true;

}

void Terrain::render(float dt)
{
    t2 = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration_cast<std::chrono::duration<float> >(t2 - t1).count();
    // Model view Projection Normal
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    set("ProjMatrix", Engine::getEngine()->graphics->projection);
    set("ViewMatrix", Engine::getEngine()->graphics->view);
    set("ModelMatrix", model);
    set("NormalMatrix", glm::mat4(1.0f));



    // Render Scale and Stuff
    set("RenderHeight", renderScale.y);
    set("MaxTextureU", float(icols)*0.1f);
    set("MaxTextureV", float(irows)*0.1f);
    set("HeightmapScale", glm::scale(glm::mat4(1.0), renderScale));
    set("Color", glm::vec4(1.0f,1.0f,1.0f,1.0f));

//    // Set Light
//    spin += dt * M_PI / 2;
//    SetLight();

    texture[0]->bind(GL_TEXTURE0);
    texture[1]->bind(GL_TEXTURE1);
    texture[2]->bind(GL_TEXTURE2);
    texture[3]->bind(GL_TEXTURE3);
    texture[4]->bind(GL_TEXTURE4);
    texture[5]->bind(GL_TEXTURE5);

    // Texture Data
    set("TextureLocations[0]", 0);
    set("TextureLocations[1]", 1);
    set("TextureLocations[2]", 2);
    set("TextureLocations[3]", 3);
    set("TextureLocations[4]", 4);
    set("TextureLocations[5]", 5);


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLM_Vertex), 0);                 // position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLM_Vertex), (const GLvoid*)12); // texture coordinate
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLM_Vertex), (const GLvoid*)20); // normal
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GLM_Vertex), (const GLvoid*)32); // tangent

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);



    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(irows*icols);
    glDrawElements(GL_TRIANGLES, isize, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisable(GL_CULL_FACE);

    RenderGrass();
}

void Terrain::CreatePositionBuffer()
{
    grass.enable();
    positions.resize(Vertices.size());
    vector<int> inactive;
    float maxDisplacement = 0.0f;
    float direction;
    float val;
    //float activate;

    //seed random number generator
    std::srand(std::time(0));


    for(uint i = 0; i < Vertices.size(); i++) {
        val = maxDisplacement * ((float) std::rand() / RAND_MAX);
        direction = (float) std::rand() / RAND_MAX;
        //activate = (float) std::rand() / RAND_MAX;

        //randomizes which grass blades are rendered
        //if (activate < 0.2) {
            cout << "pathPos r: " << pathPos[i].x << endl;
            if (direction > 0.5 && pathPos[i].x == 255) {
                positions[i] = glm::vec3(Vertices[i].position.x * renderScale.x + val *1.3 ,
                                         Vertices[i].position.y * renderScale.y,
                                         Vertices[i].position.z * renderScale.z - val*1.3);
            }
            else if(pathPos[i].x == 255)  {
                positions[i] = glm::vec3(Vertices[i].position.x * renderScale.x -val*1.25,
                                         Vertices[i].position.y * renderScale.y,
                                         Vertices[i].position.z * renderScale.z +val*1.2);
            }
        //}
    }


    glGenBuffers(1, &grass_VB);
    glBindBuffer(GL_ARRAY_BUFFER, grass_VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
}

void Terrain::ReadPath() {
    fipImage path;
    RGBQUAD pixel;
    uint height, width;


    path.load("../bin/terrain/path3.jpg");
    path.convertTo32Bits();
    height = path.getHeight();
    width = path.getWidth();

    //build position and color vectors

    for (uint i = 0; i < height; i++) {
        for (uint j = 0; j < width; j++) {
            path.getPixelColor(i, j, &pixel);
            float r, g, b;
            r = pixel.rgbRed;
            g = pixel.rgbGreen;
            b=  pixel.rgbBlue;
            //outs << r << " " << g << " " << b << endl;

            pathPos.push_back(glm::vec3(r,g,b));

            cout << "POS: " << i << "|" << j << " COLOR: " << pathPos[pathPos.size()-1].x << " " << pathPos[pathPos.size()-1].y << " " <<pathPos[pathPos.size()-1].z  << endl;
        }
    }


}


void Terrain::RenderGrass()
{
    grass.enable();
    //glDepthFunc(GL_LESS);

    glm::mat4 vp = Engine::getEngine()->graphics->projection * Engine::getEngine()->graphics->view;
    glm::vec3 camPos = Engine::getEngine()->graphics->camera->getPos();
    grass.set("model", model);

    grass.set("gVP", vp);
    grass.set("gCameraPos", camPos);
    grass.set("time", time);
    grass.set("gColorMap", 0);
    grass.set("gPathMap", 1);
    grass.set("renderScale", renderScale);
    grass.set("fMaxTextureU", float(icols));
    grass.set("fMaxTextureV", float(irows));
    grass.tex->bind(GL_TEXTURE0);
    grass.pathTex->bind(GL_TEXTURE1);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, grass_VB);
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, 0); //position

    glDrawArrays(GL_POINTS, 0, positions.size());
    glDisableVertexAttribArray(0);
}


void Terrain::bind()
{

}

void Terrain::unbind()
{

}