#include "Shader.h"
#include <fstream>

Shader::Shader(GLenum shaderType)
{
    type = shaderType;
    iShader = glCreateShader(shaderType);
}

bool Shader::initialize(string filename)
{
    ifstream fstream;
    string line;
    string shaderStr = "";

    // read in Shader data from a file
    fstream.open(filename);
    if (fstream.is_open())
    {
        getline(fstream, line);
        shaderStr += line;
        shaderStr += '\n';
        while (getline(fstream, line))
        {
            shaderStr += line;
        }
        fstream.close();

        const char *shaderData = shaderStr.c_str();

        //compile the Shader
        char buffer[512];
        GLint shader_status;
        glShaderSource(iShader, 1, &shaderData, NULL);
        glCompileShader(iShader);
        glGetShaderInfoLog(iShader, 512, NULL, buffer);

        //check the compile status
        glGetShaderiv(iShader, GL_COMPILE_STATUS, &shader_status);
        if (!shader_status)
        {

            if (type == GL_VERTEX_SHADER)
            {
                cerr << "[F] FAILED TO COMPILE VERTEX SHADER!" << endl;
            }
            else
            {
                cerr << "[F] FAILED TO COMPILE FRAGMENT SHADER!" << endl;
            }

            cout << buffer;
            cout << endl;

            return false;
        }
        return true;
    }
    else
    {
        cout << "Could not open Shader file: " << filename << endl;
        return false;
    }
}

//links shader
bool Shader::link(GLuint& program)
{
    std::cout << "SOURCE: "<< this->getShader() << std::endl;
    GLint shader_status = 0;
    glAttachShader(program,this->getShader());
    //glLinkProgram(program);

    return true;
}

GLuint Shader::getShader()
{
    return iShader;
}