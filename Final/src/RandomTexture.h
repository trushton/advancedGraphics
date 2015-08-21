#ifndef RANDOM_TEXTURE_H
#define	RANDOM_TEXTURE_H

#define GLCheckError() (glGetError() == GL_NO_ERROR)
#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#include <GL/glew.h>
#include "Terrain.h"

using namespace std;

class RandomTexture
{
public:
    RandomTexture();
    ~RandomTexture();
    bool InitRandomTexture(unsigned int Size);
    void Bind(GLenum TextureUnit);
private:
    GLuint m_textureObj;
};
#endif	/* RANDOM_TEXTURE_H */