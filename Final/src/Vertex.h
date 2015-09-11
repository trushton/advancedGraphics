#ifndef VERTEX_H
#define VERTEX_H

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Vertex
    {
        GLfloat position[3];
        GLfloat normal[3];
        GLfloat texture[3];
    };

struct GLM_Vertex
{
    glm::vec3 position;
    glm::vec2 texture;
    glm::vec3 normal;
    glm::vec3 tangent;

    GLM_Vertex(){}

    GLM_Vertex(const glm::vec3 pos, const glm::vec2 uv, const glm::vec3 norm, const glm::vec3 tan){
        position = pos;
        texture = uv;
        normal = norm;
        tangent = tan;
    }
};

struct textureVertex
{
    GLfloat position[3];
    GLfloat uv[2];
};

#endif // VERTEX_H