#include "gishandler.h"
#include <vector>
#include "renderer.h"
#include "buffer.h"
#include "Terrain.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
using namespace std;

#ifndef _SHAPE_H_
#define _SHAPE_H_
class shape : Model
{
public:
    glm::mat4 model;

    shape();
    bool load(string filename);
    void setup();
    void update(float dt);
    bool createMesh(OGRSpatialReference* sr2,glm::vec2 origin,glm::vec2 scale,Terrain& t);
    void render(glm::mat4& view, glm::mat4& projection);

private:
    renderer Renderer;
    OGRDataSource* ds;
    OGRSpatialReference* sr;
    buffer vbo_geometry;
    buffer elements;
    vector<vector<glm::vec2>> points;
    int veccount;
};
#endif