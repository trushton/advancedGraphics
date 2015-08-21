#ifndef PROGRAM_H
#define PROGRAM_H


#include <map>
#include <string>

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GL/gl.h>

class Program {
    public:

        virtual ~Program() {}

        virtual void init() = 0;
        virtual void bind() = 0;
        virtual void unbind() = 0;

        GLint getLocation(const std::string& key);

        void set(const std::string& key, float value);
        void set(const std::string& key, int value);
        void set(const std::string& key, bool value);
        void set(const std::string& key, const glm::vec2& value);
        void set(const std::string& key, const glm::vec3& value);
        void set(const std::string& key, const glm::vec4& value);
        void set(const std::string& key, const glm::mat3& value);
        void set(const std::string& key, const glm::mat4& value);

    protected:
        static GLuint loadShader(const char* fileName, GLenum shaderType);
        virtual void loadShaders() = 0;
        virtual void initShaderProgram() = 0;
        virtual void initShaderLocations() = 0;

        GLuint program;
        GLuint shaders[3];
        std::map<std::string, GLint> locations;
    };


#endif
