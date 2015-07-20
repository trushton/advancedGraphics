#include "gishandler.h"
#include <string>
#include <vector>
#include "Model.h"
#include "GBuffer.h"
#include "buffer.h"
#include "renderer.h"
#include "Engine.h"

using namespace std;

#ifndef _TERRAIN_H_
#define _TERRAIN_H_

// A Terrain class for loading DEMs from GIS data with GDAL
class Terrain : public Model
{
public:
    glm::mat4 model;


    Terrain();
    Terrain(string filename);

    //void SampleTerrain();
    void setup();
    void update(float dt);
    void render(glm::mat4& view, glm::mat4& projection);
    //void cleanup();
    void SetFile(string filename);
    float SampleTerrain(glm::vec2 point);
    glm::vec2 GetOrigin()
    {
        return origin;
    };
    OGRSpatialReference* GetProjection()
    {
        return &sr;
    }
private:
    renderer Renderer;
    float** terrainData;
    buffer elements;
    buffer terrainpoints;
    string fname, projection;
    float min, max, xres, yres;
    vector<vector<float>> vecs;
    vector<TerrainVertex> vertexes;
    vector<int> indices;
    OGRSpatialReference sr;
    glm::vec2 origin;
    glm::vec2 end;
    int width,height;

    GLuint m_Buffers[3];

};

#endif