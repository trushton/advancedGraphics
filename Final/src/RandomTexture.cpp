#include <glm/detail/type_vec3.hpp>
#include "RandomTexture.h"
#include <random>

struct R_Vec3 {
    float x,y,z;
};

RandomTexture::RandomTexture(unsigned int rand_size, GLenum type)
        : Texture(type), size(rand_size)
{
    InitRandomTexture(size);
}


RandomTexture::~RandomTexture()
{
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }
}

bool RandomTexture::InitRandomTexture(unsigned int size)
{
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> dist;

    R_Vec3 *data = new R_Vec3[size];
    for(unsigned int i = 0; i < size; i++) {
        data[i].x = dist(eng);
        data[i].y = dist(eng);
        data[i].z = dist(eng);
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_1D, textureID);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, size, 0, GL_RGB, GL_FLOAT, data);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    delete[] data;
}


void RandomTexture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_1D, textureID);
}