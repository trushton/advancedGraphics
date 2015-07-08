
#include <limits.h>
#include <string.h>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "ds_geom_pass_tech.h"
#include "ogldev_util.h"

DSGeomPassTech::DSGeomPassTech()
{
}

bool DSGeomPassTech::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "geometry_pass.vs")) {
        return false;
    }


    if (!AddShader(GL_FRAGMENT_SHADER, "geometry_pass.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }
// set up the vertex position attribute
//    loc_vertexPosition = glGetAttribLocation(shaderProgram, const_cast<const char*>("v_position"));
//    if(loc_vertexPosition == 0xFFFFFFFF)
//    {
//        cerr << "[F] POSITION NOT FOUND" << endl;
//        cout << fragmentBuffer;
//
//        return false;
//    }
//
//    // set up the vertex uv coordinate attribute
//    loc_vertexTexture = glGetAttribLocation(shaderProgram, const_cast<const char*>("v_texture"));
//    if(loc_vertexTexture == 0xFFFFFFFF)
//    {
//        cerr << "[F] V_COLOR NOT FOUND" << endl;
//        cout << fragmentBuffer;
//
//        return false;
//    }
//
//    loc_vertexNormal = glGetAttribLocation(shaderProgram, const_cast<const char*>("v_normal"));
//    if(loc_vertexNormal == 0xFFFFFFFF)
//    {
//        std::cerr << "[F] V_NORMAL NOT FOUND" << std::endl;
//        return false;
//    }
    loc_vertexPosition = glGetAttribLocation(shaderProgram, const_cast<const char*>("v_position"));
    loc_vertexTexture = glGetAttribLocation(shaderProgram, const_cast<const char*>("v_texture"));
    loc_vertexNormal = glGetAttribLocation(shaderProgram, const_cast<const char*>("v_normal"));
    loc_mvpMatrix = GetUniformLocation("mvpMatrix");
    loc_modelMatrix = GetUniformLocation("modelMatrix");
    loc_texture = GetUniformLocation("gColorMap");

    if (loc_mvpMatrix == INVALID_UNIFORM_LOCATION ||
            loc_modelMatrix == INVALID_UNIFORM_LOCATION ||
            loc_texture == INVALID_UNIFORM_LOCATION) {
        return false;
    }

    return true;
}


void DSGeomPassTech::SetMVP(const glm::mat4 mvp)
{
    glUniformMatrix4fv(loc_mvpMatrix, 1, GL_FALSE, glm::value_ptr(mvp));
}


void DSGeomPassTech::SetModelMatrix(const glm::mat4 model)
{
    glUniformMatrix4fv(loc_modelMatrix, 1, GL_FALSE, glm::value_ptr(model));
}


void DSGeomPassTech::SetTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(loc_texture, TextureUnit);
}