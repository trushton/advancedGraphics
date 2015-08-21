
#ifndef TEXTURE_H
#define	TEXTURE_H
#include <string>
#include <GL/glew.h>
#include <ImageMagick/Magick++.h>
class Texture
{
public:
    Texture(GLenum TextureTarget, const std::string& FileName);
    void Load();
    void Bind(GLenum TextureUnit);
    Magick::Image* m_pImage;
    Magick::Blob m_blob;

private:
    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
};
#endif	/* TEXTURE_H */