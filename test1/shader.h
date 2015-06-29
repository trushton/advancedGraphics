#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

class shader {

public:
    shader(GLenum);
    bool initialize(string);

    GLuint getShader ();

private:
    GLenum type;
    GLuint iShader;
};

