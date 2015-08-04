#include <glm/detail/type_vec3.hpp>
#include "RandomTexture.h"

RandomTexture::RandomTexture()
{
    m_textureObj = 0;
}
RandomTexture::~RandomTexture()
{
    if (m_textureObj != 0) {
        glDeleteTextures(1, &m_textureObj);
    }
}
bool RandomTexture::InitRandomTexture(unsigned int Size)
{
    glm::vec3* pRandomData = new glm::vec3[Size];
    for (unsigned int i = 0 ; i < Size ; i++) {
        pRandomData[i].x = (float)std::rand() / RAND_MAX;
        pRandomData[i].y = (float)std::rand() / RAND_MAX;
        pRandomData[i].z = (float)std::rand() / RAND_MAX;
    }
    glGenTextures(1, &m_textureObj);
    glBindTexture(GL_TEXTURE_1D, m_textureObj);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, Size, 0.0f, GL_RGB, GL_FLOAT, pRandomData);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    delete [] pRandomData;
    return GLCheckError();
}
void RandomTexture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_1D, m_textureObj);
}