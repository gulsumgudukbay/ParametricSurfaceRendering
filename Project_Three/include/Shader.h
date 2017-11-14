#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

class Shader
{
    public:
        Shader( const std::string& fName);
        void Bind();
        virtual ~Shader();

        GLuint my_program;
        GLuint createShader( const std::string& text, GLenum type);

        // BUREACURACY //
        void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
        std::string loadShader(const std::string& fileName);
        // END DEF

    protected:

    private:
        static const unsigned int NUM_SHADERS = 2;
        GLuint my_shaders[NUM_SHADERS];

};

#endif // SHADER_H
