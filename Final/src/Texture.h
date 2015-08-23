
#ifndef TEXTURE_H
#define	TEXTURE_H
#include <string>
#include <GL/glew.h>
#include <ImageMagick/Magick++.h>
#include <string>


class Texture {
public:
    Texture(GLenum type);
    Texture(const std::string& fileName, GLenum type);
    virtual ~Texture();

    virtual void init();
    virtual void bind(GLenum textureUnit);

protected:
    std::string file_name;
    GLenum target;
    GLuint textureID;
};
#endif	/* TEXTURE_H */