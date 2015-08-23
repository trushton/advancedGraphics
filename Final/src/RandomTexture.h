#ifndef RANDOM_TEXTURE_H
#define RANDOM_TEXTURE_H


#include "GL/glew.h"
#include "Texture.h"
#include <time.h>
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/gtc/matrix_transform.hpp>

class RandomTexture : public Texture
{
public:
    RandomTexture(unsigned int rand_size, GLenum type);

    ~RandomTexture();

    bool InitRandomTexture(unsigned int Size);

    void Bind(GLenum TextureUnit);

private:
    unsigned int size;

    //GLuint m_textureObj;
};


#endif