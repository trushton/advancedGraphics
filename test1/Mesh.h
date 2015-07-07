#include <GL/glew.h>
#include <GL/glut.h> 
#include <vector>
#include "Vertex.h"
#include <FreeImagePlus.h>

class Mesh {

    public:

        void setData();
        
        std::vector<textureVertex> vertices;
        std::vector<unsigned int> indices;

        GLuint VB;
        GLuint IB;
        GLuint TB;
        unsigned int numIndices;
        unsigned int materialIndex;

        fipImage image;
};