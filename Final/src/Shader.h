#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>


#include <iostream>
#include <string>


using namespace std;

class Shader
    {

    public:
        Shader(GLenum);

        bool initialize(string);

        GLuint getShader();

        bool link(GLuint& program);


private:
        GLenum type;
        GLuint iShader;
    };

#endif
